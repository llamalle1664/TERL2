#ifndef DEF_RENDERER_HPP
#define DEF_RENDERER_HPP

#include <vector>

#include <SFML/Graphics/VertexBuffer.hpp>

namespace sf {
	class RenderTarget;
}
class World;
class AgentData;

class Renderer {
public:
	Renderer(World const& world);

	/*
	 * Update the walls vertex buffer with the circuit given
	 * Only need to call this when the circuit changes
	 */
	void updateCircuit(World const& world);

	/*
	 * Sets the number of agents to display
	 * Must be <= tothe number of agents in the simulator
	 */
	void setAgentCount(int count);

	/*
	 * Update the agents buffer with the position of the agents
	 * in the vector
	 * Needs to be called everytime the positon of an agent changes
	 */
	void updateAgents(std::vector<AgentData> const& data);

	void render(sf::RenderTarget& target);

private:
	// vertex buffer for the circuit
	sf::VertexBuffer m_circuit_vbuffer;

	/* 
	 * vertex buffer for all the agents
	 * contains only the vertex data for 1 agent
	 */
	sf::VertexBuffer m_agent_alive_vbuffer;
	sf::VertexBuffer m_agent_dead_vbuffer;

	/*
	 * vertex buffer for the sensors
	 * contains only the vertex data for 1 sensor array
	 */
	sf::VertexBuffer m_sensors_vbuffer;

	std::vector<sf::Transform> m_agent_transform;
	std::vector<bool> m_agent_alive;

	int m_agent_count;
};

#endif
