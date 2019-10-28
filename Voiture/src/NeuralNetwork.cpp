#include <cmath>
#include <iostream>

#include "NeuralNetwork.hpp"
#include "utils.hpp"

/*Creates a feed forward neural network
 *config - layer configuration : {3, 5, 5, 2} creates a network with 3 input, 2 hidden layers of 5 neurons, and 2 outputs
 */

 //Makes a random neural network
NeuralNetwork::NeuralNetwork(std::vector<int> const& config):
	neurons(config.size()),
	m_config(config),
	m_size(0)
{
	for (size_t i = 0; i < config.size(); i++) {
		neurons[i] = Layer(config[i]);
		for (int j = 0; j < config[i]; j++) {
			initNeuron(neurons[i][j], i);
		}
	}

	for (auto&& v: m_config)
		m_size += v;

	randomize(NNetwork_randomFactor);
	makeGenes();
}

NeuralNetwork::NeuralNetwork(NeuralNetwork* a, NeuralNetwork* b):
	neurons(a->getConfig().size()),
	m_config(a->getConfig()),
	m_size(0)
{
	auto const& wa = a->getGeneWeights();
	auto const& wb = b->getGeneWeights();
	auto const& na = a->getGeneBiases();
	auto const& nb = b->getGeneBiases();
	if (wa.size() != wb.size() || na.size() != nb.size()) {
		throw std::runtime_error("Genes arent compatible, genes must have the same lenght");
	}
	//Make the brain
	for (size_t i = 0; i < m_config.size(); i++) {
		neurons[i] = Layer(m_config[i]);
		for (int j = 0; j < m_config[i]; j++) {
			initNeuron(neurons[i][j], i);
		}
	}
	//Mix neurons values
	uint i = 0;
	while (i < na.size()) {
		for (int x = randRange(NNetwork_minMixNeuron, NNetwork_maxMixNeuron); i < na.size() && x > 0; x--) {
			m_biases.push_back(na[i]);
			i++;
		}
		for (int x = randRange(NNetwork_minMixNeuron, NNetwork_maxMixNeuron); i < na.size() && x > 0; x--) {
			m_biases.push_back(nb[i]);
			i++;
		}
	}
	//Mix weights values
	i = 0;
	while (i < wa.size()) {
		for (int x = randRange(NNetwork_minMixWeight, NNetwork_maxMixWeight); i < wa.size() && x > 0; x--) {
			m_weights.push_back(wa[i]);
			i++;
		}
		for (int x = randRange(NNetwork_minMixWeight, NNetwork_maxMixWeight); i < wa.size() && x > 0; x--) {
			m_weights.push_back(wb[i]);
			i++;
		}
	}
	mutate();
}

std::vector<float> const& NeuralNetwork::getGeneWeights() const {
	return m_weights;
}

std::vector<float> const& NeuralNetwork::getGeneBiases() const {
	return m_biases;
}

int NeuralNetwork::randRange (int min, int max) const {
	if (min > max) {
		int c = min;
		min = max;
		max = c;
	}
	return rand() % (max - min) + min;
}

//Makes a neural network based on two genes vector<float>
//cf; makeGenes()
void NeuralNetwork::setGenes(std::vector<float> const& weightsArray, std::vector<float> const& biasesArray) {
	m_weights = weightsArray;
	m_biases = biasesArray;

	auto cw = m_weights.begin();
	auto cn = m_biases.begin();
	for (auto&& l: neurons) {
		for (auto&& n: l) {
			n.bias= *cn;
			++cn;
			for (auto& w: n.weights) {
				w = *cw;
				++cw;
			}
		}
	}
}

//Randomises the network based on a factor
void NeuralNetwork::randomize(float factor) {
	for (auto& layer: neurons) {
		for (auto& neuron: layer) {
			randomizeNeuron(neuron, factor);
		}
	}

	makeGenes();
}

void NeuralNetwork::mutate() {
		for (int i = randRange(0, NNetwork_maxMut); i < (int)m_biases.size(); i += randRange(NNetwork_minMut, NNetwork_maxMut)) {
			m_biases[i] += fRand() * NNetwork_randomFactor;
		}
		for (int i = randRange(0, NNetwork_maxMut); i < (int)m_weights.size(); i += randRange(NNetwork_minMut, NNetwork_maxMut)) {
			m_weights[i] += fRand() * NNetwork_randomFactor;
		}
		setGenes(m_weights, m_biases);
}

//Propagate values forwards
void NeuralNetwork::evaluate() {
	for (size_t i = 1; i < neurons.size(); i++) {		//for each layers
		for (auto& neuron: neurons[i]) {			//for each neurons on that layer
			neuron.value = 0.f;					//Reseting the value of the neuron
			//for each neurons from the previous layer
			for (size_t k = 0; k < neurons[i - 1].size(); k++) {
				//Adding the neuron based on the weight array
				neuron.value += neurons[i - 1][k].value * neuron.weights[k];
			}
			//passing the value and threshold into a sigmoid function
			neuron.value = tanh(neuron.value - neuron.bias);
		}
	}
}

/*Sets the intput "i" with the value "value"
 *Returns true if the value has been set, false if not
 */
bool NeuralNetwork::setInput(size_t i, float value) {
	if (neurons.empty() ||
		i >= neurons[0].size()) {
		return false;
	}
	neurons[0][i].value = value;
	return true;
}
/*Gets the output "i"
 *Returns the value of the output if it exists, 0 if not
*/
float NeuralNetwork::getOutput(size_t i) const {
	if (neurons.empty() ||
		i >= neurons.back().size()) {
		return 0.0f;
	}
	return neurons.back()[i].value;
}

//Inits a neuron
void NeuralNetwork::initNeuron(Neuron& n, int layer)
{
	n = Neuron();
	//a neuron has a connection to every neurons of the previous layer
	if (layer > 0) // input neurons have no connection to previous neurons
		n.weights = std::vector<float>(neurons[layer-1].size());
}

//Randomise a neuron weights and threshold based on a factor
void NeuralNetwork::randomizeNeuron(Neuron& n, float factor) {
	for (auto& weight: n.weights)
		weight = fRand() * factor;
	n.bias= fRand() * factor;
}

//Returns a float on the range[-1.0 ; 1.0]
float NeuralNetwork::fRand() const {
	return 1 - ((float)(rand()) / (float)(RAND_MAX)) * 2;
}

//Prints the values from the neurons
void NeuralNetwork::print() const {
	for (auto& layer: neurons) {
		for (auto& neuron: layer) {
			std::cout << neuron.bias<< " ";
		}
		std::cout << "\n\n";
	}
}

void NeuralNetwork::printGene() const {
	std::cout << "Biases: ";
	for (auto& n: m_biases) {
		std::cout << n << " ";
	}
	std::cout << std::endl << "Weights: ";
	for (auto& w: m_weights) {
		std::cout << w << " ";
	}
	std::cout << '\n';
}

//Stores the neural network into two vector<float>
// one used for the thresholds
// one for the weights
//those arrays can be used to mutate or make new NeuralNetwork easely
void NeuralNetwork::makeGenes() {
	m_weights.clear();
	m_biases.clear();
	for (auto&& l: neurons) {
		for (auto&& n: l) {
			m_biases.push_back(n.bias);
			for (auto&& w: n.weights) {
				m_weights.push_back(w);
			}
		}
	}
}

std::vector<int> const& NeuralNetwork::getConfig() const {
	return m_config;
}

size_t NeuralNetwork::getSize() const {
	return m_size;
}
