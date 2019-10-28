#include "Renderer.hpp"

#include <vector>
#include <cmath>

#include <cstdlib>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "World.hpp"
#include "utils.hpp"

Renderer::Renderer(World const& world):
	m_circuit_vbuffer(sf::Lines, sf::VertexBuffer::Usage::Static),
	m_agent_alive_vbuffer(sf::Triangles, sf::VertexBuffer::Usage::Static),
	m_agent_dead_vbuffer(sf::Triangles, sf::VertexBuffer::Usage::Static),
	m_sensors_vbuffer(sf::Lines, sf::VertexBuffer::Usage::Static)
{
	setAgentCount(100);

	// init the agent buffer
	constexpr float pi = std::acos(-1.f);
	std::vector<sf::Vertex> vbuffer;
	for (int i = 1; i <= agent_point_count; i++) {
		const float angle  = i * 2.f*pi/agent_point_count;
		const float anglep = (i-1) * 2.f*pi/agent_point_count;
		vbuffer.emplace_back(sf::Vector2f(0,0), sf::Color::Green);
		vbuffer.emplace_back(sf::Vector2f(std::cos(angle), std::sin(angle))*agent_radius, sf::Color::Green);
		vbuffer.emplace_back(sf::Vector2f(std::cos(anglep), std::sin(anglep))*agent_radius, sf::Color::Green);
	}
	vbuffer.emplace_back(sf::Vector2f(0, 0.1f*agent_radius), sf::Color::Blue);
	vbuffer.emplace_back(sf::Vector2f(0,-0.1f*agent_radius), sf::Color::Blue);
	vbuffer.emplace_back(sf::Vector2f(agent_radius, 0), sf::Color::Blue);

	m_agent_alive_vbuffer.create(vbuffer.size());
	m_agent_alive_vbuffer.update(vbuffer.data());

	for (int i = 0; i < vbuffer.size() - 3; i++)
		vbuffer[i].color = sf::Color::Red;

	m_agent_dead_vbuffer.create(vbuffer.size());
	m_agent_dead_vbuffer.update(vbuffer.data());

	// init the sensors buffer
	vbuffer.clear();
	for (auto&& s: world.getSensors()) {
		vbuffer.emplace_back(s.a, sf::Color::Yellow);
		vbuffer.emplace_back(s.b, sf::Color::Yellow);
	}

	m_sensors_vbuffer.create(vbuffer.size());
	m_sensors_vbuffer.update(vbuffer.data());
}

void Renderer::setAgentCount(int agent_count) {
	m_agent_count = agent_count;
	m_agent_transform.resize(agent_count);
	m_agent_alive.resize(agent_count);
}

void Renderer::updateCircuit(World const& world) {
	auto const& walls = world.getWalls();
	auto const& checkpoints = world.getCheckpoints();

	std::vector<sf::Vertex> vbuffer;
	for (auto&& w: walls) {
		vbuffer.emplace_back(w.a, sf::Color::Red);
		vbuffer.emplace_back(w.b, sf::Color::Red);
	}
	for (auto&& c: checkpoints) {
		vbuffer.emplace_back(c.body.a, sf::Color::Green);
		vbuffer.emplace_back(c.body.b, sf::Color::Green);
	}
	m_circuit_vbuffer.create(vbuffer.size());
	m_circuit_vbuffer.update(vbuffer.data());
}

void Renderer::updateAgents(std::vector<AgentData> const& data) {
	if (m_agent_transform.size() < data.size()) {
		m_agent_transform.resize(data.size());
		m_agent_alive.resize(data.size());
	}

	sf::Transformable t;
	for (size_t i = 0; i < data.size(); i++) {
		t.setRotation(data[i].rot);
		t.setPosition(data[i].pos);
		m_agent_alive[i] = data[i].alive;
		if (data[i].alive)
			m_agent_transform[i] = t.getTransform();
	}
}

void Renderer::render(sf::RenderTarget& target) {
	// draw circuit
	target.draw(m_circuit_vbuffer);

	// draw agents and sensors
	for (int i = 0; i < m_agent_count; i++) {
		if (m_agent_alive[i])
			target.draw(m_agent_alive_vbuffer, m_agent_transform[i]);
		else
			target.draw(m_agent_dead_vbuffer, m_agent_transform[i]);
		target.draw(m_sensors_vbuffer, m_agent_transform[i]);
	}
}
