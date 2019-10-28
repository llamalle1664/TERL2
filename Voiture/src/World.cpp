#include "World.hpp"

#include <cmath>

#include <SFML/Graphics/Transformable.hpp>

#include "NeuralNetwork.hpp"

#include <iostream>
#include <fstream>

World::World():
	m_agent_pool(-1)
{
	// setup sensors
	constexpr float sensor_step = M_PI/static_cast<float>(agent_sensor_count-1);
	float angle = -M_PI/2.f;
	for (int i = 0; i < agent_sensor_count; i++) {
		const sf::Vector2f u(std::cos(angle), std::sin(angle));
		m_sensors[i].a = u*agent_radius;
		m_sensors[i].b = u*(agent_radius + agent_sensor_range);
		angle += sensor_step;
	}
}

World::~World() {
}

void World::save(std::string const& path) const {
	std::ofstream file(path);

	if (!file) {
		std::cerr << "Can't save world to file [" << path << "]\n";
		return;
	}

	for (auto&& w: m_walls)
		file << w.a.x << " " << w.a.y << " " << w.b.x << " " << w.b.y << "\n";

	file << "==== ==== ==== ====\n";

	for (auto&& cp: m_checkpoints)
		file << cp.id << " " << 
		        cp.body.a.x << " " <<
		        cp.body.a.y << " " <<
		        cp.body.b.x << " " <<
		        cp.body.b.y << "\n";
}

bool World::load(std::string const& path) {
	std::ifstream file(path);
	if (!file) {
		std::cerr << "Can't load world from file [" << path << "]\n";
		return false;
	}

	std::vector<Wall> walls;
	std::vector<Checkpoint> checkpoints;

	try {
		std::string a, b, c, d;
		while (!file.eof()) {
			file >> a >> b >> c >> d;
			if (a == "====" &&
					b == "====" &&
					c == "====" &&
					d == "====")
				break;
			Wall w;
			w.a.x = std::stof(a);
			w.a.y = std::stof(b);
			w.b.x = std::stof(c);
			w.b.y = std::stof(d);
			walls.push_back(w);
		}

		while (!file.eof()) {
			Checkpoint c;
			file >> c.id;
			file >> c.body.a.x;
			file >> c.body.a.y;
			file >> c.body.b.x;
			file >> c.body.b.y;
			checkpoints.push_back(c);
		}
	}
	catch (std::exception& e) {
		std::cerr << "Error while loading world from [" << path << "]:\n";
		std::cerr << e.what() << std::endl;
		return false;
	}

	m_walls = walls;
	m_checkpoints = checkpoints;

	return true;
}

void World::clear() {
	m_walls.clear();
	m_checkpoints.clear();
}

void World::resetAgents(std::vector<NeuralNetwork*> const& networks) {
	m_networks = networks;
	m_data = std::vector<AgentData>(networks.size());
	m_agent_pool = networks.size() - 1;
}

std::vector<AgentData> const& World::simulate() {
	for (int i = 0; i <= m_agent_pool; i++) {
		workerStep(i, world_worker_timeout);
	}

	return m_data;
}

std::vector<AgentData> const& World::step(float dt) {
	for (int i = 0; i <= m_agent_pool; i++) {
		auto& agent = m_data[i];
		if (!agent.alive)
			continue;
		stepOnce(dt, i);

		agent.timer += dt;
		agent.max_timer += dt;

		// on timeout
		if (agent.timer > world_worker_timeout) {
			// is the agent is still in the same sector, kill it
			if (agent.checkpoint_before_timeout == agent.last_sector_id) {
				agent.alive = false;
				continue;
			}
			// else just reset the timer and sector
			else {
				agent.timer = 0.f;
				agent.checkpoint_before_timeout = agent.last_sector_id;
			}
		}
	}

	return m_data;
}

std::vector<Wall> const& World::getWalls() const {
	return m_walls;
}

std::vector<Checkpoint> const& World::getCheckpoints() const {
	return m_checkpoints;
}

std::array<Wall, agent_sensor_count> const& World::getSensors() const {
	return m_sensors;
}

bool World::collision(AgentData const& agent, Wall const& wall) const {
	const sf::Vector2f A = agent.pos - wall.a;
	const sf::Vector2f B = wall.b - wall.a;

	// unit vector of B
	const sf::Vector2f Bu = B/std::sqrt(B.x*B.x + B.y*B.y);

	// projection of A on B
	const sf::Vector2f a1 = (A.x*Bu.x + A.y*Bu.y)*Bu;

	// rejection of A on B
	const sf::Vector2f a2 = A - a1;

	// if length of rejection is < radius, there is a collision
	if (a2.x*a2.x + a2.y * a2.y <= agent_radius*agent_radius) {
		// if agent inside wall endpoint
		sf::Vector2f e1(agent.pos - wall.a), e2(agent.pos - wall.b);
		if (e1.x*e1.x + e1.y*e1.y <= agent_radius*agent_radius ||
		    e2.x*e2.x + e2.y*e2.y <= agent_radius*agent_radius)
			return true;

		// if the projection point is inside the segment
		float d1 = a1.x*a1.x + a1.y*a1.y;
		sf::Vector2f t = B - a1;
		float d2 = t.x*t.x + t.y*t.y;
		float d = B.x*B.x + B.y*B.y;

		return d1+d2-d <= 0.1f;
	}
	return false;
}

float World::raycast(sf::Vector2f const& beg, sf::Vector2f const& end) const {
	float min_ray = 1.f;

	const sf::Vector2f s1 (end - beg);

	// TODO: use space partitioning for this
	for (auto&& w: m_walls) {
		const sf::Vector2f s2(w.b - w.a);

		const float s = (-s1.y * (beg.x - w.a.x) + s1.x * (beg.y - w.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
		const float t = ( s2.x * (beg.y - w.a.y) - s2.y * (beg.x - w.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

		// on collision
		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			min_ray = std::min(t, min_ray);
		}
	}

	return min_ray*2.f - 1.f;
}

void World::workerStep(const int index, float timeout, float step) {
	auto& agent = m_data[index];

	// step until agent is dead
	while (!stepOnce(step, index)) {
		agent.timer += step;
		agent.max_timer += step;

		// on timeout
		if (agent.timer > timeout) {
			// is the agent is still in the same sector, kill it
			if (agent.checkpoint_before_timeout == agent.last_sector_id) {
				agent.alive = false;
				break;
			}
			// else just reset the timer and sector
			else {
				agent.timer = 0.f;
				agent.checkpoint_before_timeout = agent.last_sector_id;
			}
		}

		// on sim timeout
		if (agent.max_timer >= world_worker_sim_max_time) {
			// agent is still alive, so just break
			break;
		}
	}
}

bool World::stepOnce(float dt, int index) {
	auto& agent = m_data[index];
	NeuralNetwork* nn = m_networks[index];

	// position + rotation transfrom matrix for the agent
	sf::Transformable transform;
	transform.setRotation(agent.rot);
	transform.setPosition(agent.pos);
	const auto t = transform.getTransform();

	// set the input values from the raycasts of the sensors
	for (int i = 0; i < agent_sensor_count; i++) {
		nn->setInput(i, raycast(t*m_sensors[i].a, t*m_sensors[i].b));
	}

	// get output
	nn->evaluate();
	const float output = nn->getOutput(0);

	// update position and rotation
	agent.rot += output * agent_turn_speed * dt;
	agent.pos += 
		sf::Vector2f(std::cos(agent.rot*M_PI/180.f), std::sin(agent.rot*M_PI/180.f)) * agent_speed * dt;

	// first update the last sector
	for (auto&& c: m_checkpoints) {
		if (collision(agent, c.body)) {
			if (c.id > agent.last_sector_id)
				agent.sector_count++;
			else {
				if (c.id == 0 && agent.last_sector_id != 0)
					agent.sector_count++;
			}
			agent.last_sector_id = c.id;
			break;
		}
	}

	// update tick
	agent.tick_count++;

	// then check collisions with the walls
	// TODO: use space partitioning for this
	for (auto&& w: m_walls) {
		if (collision(agent, w)) {
			agent.alive = false;
			return true; // kill the agent on collision
		}
	}

	return false;
}

void World::addWall(Wall const& w) {
	m_walls.push_back(w);
}

void World::addCheckpoint(Checkpoint const& p) {
	m_checkpoints.push_back(p);
}

void World::removeLastWall() {
	if (m_walls.empty())
		return;
	m_walls.pop_back();
}

void World::removeLastCheckpoint() {
	if (m_checkpoints.empty())
		return;
	m_checkpoints.pop_back();
}
