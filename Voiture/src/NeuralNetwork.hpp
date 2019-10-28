#ifndef _NEURALNETWORK
#define _NEURALNETWORK

#include <vector>
#include <cstddef>

struct Neuron {
	Neuron(): value(0.f), bias(1.f) {}
	float value;
	float bias;
	std::vector<float> weights;
};

class NeuralNetwork {
public:
	typedef std::vector<Neuron> Layer;

	//Attributes
private:
	std::vector<float> m_weights;
	std::vector<float> m_biases;
	std::vector<Layer> neurons;
	std::vector<int> m_config;
	size_t m_size;

	//Constructors
public:
	//Random NN
	NeuralNetwork(std::vector<int> const&);
	//Child NN
	NeuralNetwork(NeuralNetwork* a, NeuralNetwork* b);
	/*
	 * randomize the weights of the network using @factor
	 */
	void randomize (float factor);

	void mutate();
	/*
	 * Compute the output values from the input
	 */
	void evaluate ();

	// set input values used for the next eval
	bool setInput (size_t i, float value);

	// get the output values from the last eval
	float getOutput (size_t i) const;

	void print () const;
	void printGene() const;


	// internal use
	void setGenes(std::vector<float> const& weightsArray, std::vector<float> const& biasesArray);
	void makeGenes();

	// getter
	std::vector<int> const& getConfig() const;
	size_t getSize() const;
	std::vector<float> const& getGeneBiases() const;
	std::vector<float> const& getGeneWeights() const;

private:
	void initNeuron (Neuron& n, int layer);
	void randomizeNeuron(Neuron& n, float factor);
	float fRand () const;
	int randRange (int, int) const;
};

#endif
