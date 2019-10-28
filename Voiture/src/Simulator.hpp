#ifndef DEF_SIMULATOR_HPP
#define DEF_SIMULATOR_HPP

#include <atomic>
#include <vector>

#include "utils.hpp"
#include "NeuralNetwork.hpp"
#include "World.hpp"

//class NeuralNetwork;
//class World;

struct Agent {
	Agent(NeuralNetwork* nn):
	brain(nn),
	data(AgentData()),
	score(0),
	probability(0)
	{
	}
	NeuralNetwork* brain;
	AgentData data;
	float score;
	float probability;
};

class Plot;
class Simulator {
public:
	Simulator(Plot& plot, World& world, std::vector<int> const& config);

	void run();
	void runForGen(uint x);
	void stop();
	std::vector<NeuralNetwork*> getGeneration(int n);
	size_t getHistorySize() const;

	bool isRunning() const;

private:
	void simulate();
	void nextGeneration();
	void rateAgents();
	void makeLoveTable();
	void sweetLove(std::vector<Agent>&);
	float randZeroOne() const;
  	void logFile(bool);

private:
	std::atomic_bool run_flag;
	std::vector<Agent> generation;
	std::vector<NeuralNetwork*> networks;
	std::vector<int> m_config; // configuration of the NNs
	std::vector<std::vector<NeuralNetwork*>> history;
	World&  m_world;
	Plot& m_plot;
};

#endif
