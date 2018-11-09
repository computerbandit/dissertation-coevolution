#include "GeneticNN.h"

Chromeosome<float> GeneticNN::ConvertConnectionNetworkToChromosome()
{
	std::vector<float> sequence = std::vector<float>();
	for (ConnectionArray layer : this->_connectionNetwork)
		for (ConnectionWeight w : layer)
			sequence.push_back(w.GetWeight());

	return Chromeosome<float>(sequence);
}

void GeneticNN::LoadConnectionNetworkFromChromosome(const Chromeosome<float>& chromesome)
{

}
