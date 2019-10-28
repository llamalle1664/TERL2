#ifndef DEF_WORLD_HPP
#define DEF_WORLD_HPP

#include <array>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "utils.hpp"

struct Wall {
	Wall () {}
	Wall (sf::Vector2f const& a, sf::Vector2f const& b): a(a), b(b) {}
	sf::Vector2f a, b;
};

struct Checkpoint {
	Checkpoint(): id(0) {}
	Checkpoint(int id, Wall const& b): id(id), body(b) {}
	int  id;
	Wall body;
};

struct AgentData {
	AgentData():
		rot(0.f),
		last_sector_id(0),
		alive(true),
		sector_count(0),
		tick_count(0),
		distance_to_last_sector(0.f),
		timer(0.f),
		max_timer(0.f),
		checkpoint_before_timeout(0)
	{
	}

	sf::Vector2f pos;   // position
	float        rot;   // rotation
	int last_sector_id; // last sector the agent was in

	// data for agent eval
	bool alive;       // is the agent alive?
	int sector_count; // number of sectors passed
	int tick_count;   // nmuber of simulation tick before the agent died
	float distance_to_last_sector;

	// data for world status
	float timer, max_timer;
	int checkpoint_before_timeout;
};

class NeuralNetwork;

class World {
public:
	World();
	~World();

	/*
	 * Save and load terrain to/from file
	 *
	 * Does not modify the world is load fails
	 */
	void save(std::string const& path) const;
	bool load(std::string const& path);
	
	// clear all walls/checkpoints
	void clear();

	/*
	 * Reset the agents with new brains
	 * Also reset the position/rotation and state of all agents
	 */
	void resetAgents(std::vector<NeuralNetwork*> const& networks);

	/*
	 * Simulate every agents until they all die or the simulation times out
	 */
	std::vector<AgentData> const& simulate();

	/*
	 * step all agents by @dt seconds
	 */
	std::vector<AgentData> const& step(float dt);

	std::vector<Wall> const& getWalls() const;
	std::vector<Checkpoint> const& getCheckpoints() const;
	std::array<Wall, agent_sensor_count> const& getSensors() const;

	/*
	 * For the world editor
	 */
	void addWall(Wall const& w);
	void addCheckpoint(Checkpoint const& p);
	void removeLastWall();
	void removeLastCheckpoint();

private:
	/*
	 * Check for a collision between an agent and a wall
	 *
	 * returns true if there is a collision, false otherwise
	 */
	bool collision(AgentData const& agent, Wall const& wall) const;

	/*
	 * Cast a ray from beg to end on the circuit
	 *
	 * return a value between [-1,1], -1 being beg, 1 being end or no collision
	 */
	float raycast(sf::Vector2f const& beg, sf::Vector2f const& end) const;

	/*
	 * Steps the world for an agent until it dies or @timeout is reached
	 *
	 * timeout is the maximum time allowed to reach the next checkpoint
	 */
	void workerStep(const int index, float timeout, float step = world_step);

	/*
	 * Steps the simulation for the agent at index @index once
	 * using a time step of @dt
	 *
	 * returns true if the agent died, false otherwise
	 */
	bool stepOnce(float dt, int index);

private:
	// circuit data
	std::vector<Wall> m_walls;
	std::vector<Checkpoint> m_checkpoints;

	/*
	 * Networks and data for the agents in the world
	 */
	std::vector<NeuralNetwork*> m_networks;
	std::vector<AgentData>      m_data;

	std::array<Wall, agent_sensor_count> m_sensors;

	int m_agent_pool;
};

#endif
