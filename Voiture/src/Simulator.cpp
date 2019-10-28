#include "Simulator.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "NeuralNetwork.hpp"
#include "Plot.hpp"
#include "utils.hpp"
#include "World.hpp"

Simulator::Simulator(Plot& plot, World& world, std::vector<int> const& config):
	run_flag(false),
	m_config(config),
	m_world(world),
	m_plot(plot)
{
	for (int i = 0; i < sim_genTotal; i++) {
		generation.push_back(Agent(new NeuralNetwork(m_config)));
	}
	history = std::vector<std::vector<NeuralNetwork*>>();
	remove("AvgLog.txt");
}

bool Simulator::isRunning() const {
	return run_flag;
}

void Simulator::run() {
	run_flag = true;
	while (run_flag) {
		simulate();
	}
}

void Simulator::runForGen(uint x) {
	for (uint i = 0; i < x; i++) {
		simulate();
	}
}

void Simulator::stop() {
	run_flag = false;
}

//Main function, it simulate the generation and make one based on it
void Simulator::simulate() {
	//Copy brains into a vector,(except for elders, they dont need to be simulated again)
	networks.clear();
	for (int i = 0; i < sim_genTotal; i++) {
		auto& nn = generation[i].brain;
		networks.push_back(nn);
	}
	//Simulate
	m_world.resetAgents(networks);
	auto agentD = m_world.simulate();

	//Store the simulated neural networks into the history
	history.push_back(std::vector<NeuralNetwork*>());
	for (uint i = 0; i < networks.size(); i++) {
		history[history.size() - 1].push_back(networks[i]);
	}

	//Copy data into the agents
	if (agentD.size() != generation.size()) {
		throw std::runtime_error("Data doesnt match agents");
	}

	for (int i = 0; i < generation.size(); i++) {
		generation[i].data = agentD[i];
	}

	//Rate the agents
	rateAgents();

	//LogFile
	logFile(true);

	float total = 0;
	for (auto& a: generation) {
		total += a.score;
	}
	float avg = total / sim_genTotal;
	m_plot.append(avg);
	
	//Next generation
	std::vector<Agent> newGen;
		//best ones from the generation
	for (int i = 0; i < sim_genElders; i++) {
		newGen.push_back(generation[i]);
	}
		//Reproduction + Mutation
	makeLoveTable();
	for (int i = 0; i < sim_genChilds; i++) {
		sweetLove(newGen);
	}
		//Random agent
	for (int i = 0; i < sim_genRandom; i++) {
		newGen.push_back(Agent(new NeuralNetwork(m_config)));
	}

	generation = newGen;
}

std::vector<NeuralNetwork*> Simulator::getGeneration(int n) {
	if (history.empty())
		return {};
	if (n < 0 || n > history.size())
		return history.back();

	return history[n];
}

void Simulator::logFile (bool b) {
  if (!b)
    return;
  std::ofstream file;
  file.open("AvgLog.txt", std::ios::app);
  float total = 0;
  for (auto& a: generation) {
    total += a.score;
  }
  float avg = total / sim_genTotal;
  file << avg << std::endl;
  file.close();
}

size_t Simulator::getHistorySize() const {
	return history.size();
}

//fill the "probability" field of the struct Agent
//probability represent a range from 0 to 1, where values cummulate
//if we do a rand on the range [0; 1], it points the first place where the rand is < than the value
//
//  1     2     3     4     5      6       index
// 0.5 | 0.7 | 0.8 | 0.9 | 0.95 | 1.0      values
//              ^                          if the rand = 0.75
void Simulator::makeLoveTable() {
	float total = 0;
	for (auto& a: generation) {
		total += a.score;
	}
	float combinedValues = 0;
	//std::cout << "LOVETABLE\n" << std::endl;
	for (auto& a: generation) {
		combinedValues += a.score / total;
		a.probability = combinedValues;
		//std::cout << "**\nproba: " << a.probability << "\nscore: " << a.score << " " << total << std::endl;
	}
}

//The function that gives a score to the agents and sort them by score descending
//It skips the elders that would have been already rated at some point
void Simulator::rateAgents() {
	for (int i = 0; i < sim_genTotal; i++) {
		auto& a = generation[i];
		a.score = (float)a.data.sector_count * (float)a.data.tick_count; //??
		if (a.data.alive)
			a.score *= 2;
	}
	std::sort(generation.begin(), generation.end(), [](auto&& a, auto&& b) {return a.score > b.score;});
}

//Pick two agents from the generation and makes a new one based on them and add it to gen
void Simulator::sweetLove(std::vector<Agent>& gen) {
	auto& a = generation[0];
	auto& b = generation[0];
	float r = randZeroOne();
	for (auto& agent: generation) {
		if (agent.probability > r) {
			a = agent;
		}
	}
	r = randZeroOne();
	for (auto& agent: generation) {
		if (agent.probability > r) {
			b = agent;
		}
	}
	gen.push_back(Agent(new NeuralNetwork(a.brain, b.brain)));
}

//Returns a float on the range[0.0 ; 1.0]
float Simulator::randZeroOne() const {
	return ((float)(rand()) / (float)(RAND_MAX));
}
