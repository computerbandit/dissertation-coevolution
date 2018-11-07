#pragma once
#include "NeuralNetwork.h"
#include "Chromeosome.h"

class GeneticNN : public NeuralNetwork {
public:
	~GeneticNN() {}

	Chromeosome<float> ConvertConnectionNetworkToChromosome();
	void LoadConnectionNetworkFromChromosome(const Chromeosome<float>& chromesome);

private:

	Chromeosome<float>& chromeosome;

};