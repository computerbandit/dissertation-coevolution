#pragma once
#include "NeuralNetwork.h"
#include "Chromeosome.h"

class GeneticNN : public NeuralNetwork {
public:
	GeneticNN(std::vector<int> nodesPerLayer, std::string filePath);
	GeneticNN(std::string filePath);
	~GeneticNN() {}

	static Chromeosome<float> ConvertConnectionNetworkToChromosome(GeneticNN* network);

	void LoadConnectionNetworkFromChromosome(Chromeosome<float>& chromesome);

private:
	Chromeosome<float>* _chromeosome;
};