#include "GeneticNN.h"

GeneticNN::GeneticNN(std::vector<int> nodesPerLayer, std::string filePath): NeuralNetwork(nodesPerLayer, filePath)
{
	this->_chromeosome = nullptr;
}

GeneticNN::GeneticNN(std::string filePath): NeuralNetwork(filePath)
{
	this->_chromeosome = nullptr;
}

Chromeosome<float> GeneticNN::ConvertConnectionNetworkToChromosome(GeneticNN* network)
{
	std::vector<float> sequence = std::vector<float>();
	for (ConnectionArray& layer : network->_connectionNetwork)
		for (ConnectionWeight& w : layer)
			sequence.push_back(w.GetWeight());

	return Chromeosome<float>(sequence);
}

void GeneticNN::LoadConnectionNetworkFromChromosome(Chromeosome<float>& chromesome)
{
	//like the network together from the chromeosome that has been passed in
	this->_chromeosome = &chromesome;
	std::vector<float>& sequence = _chromeosome->GetSequence();
	std::vector<float> layerSizes = std::vector<float>();
	int sequenceIndex = 0;
	for (ConnectionArray& layer : _connectionNetwork) {
		for (int i = 0; i < (int)layer.size(); i++) {
			layer[i].SetWeight(sequence[sequenceIndex]);
			sequenceIndex++;
		}
	}
}
