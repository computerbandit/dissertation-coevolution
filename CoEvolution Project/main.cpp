#include <iostream>
#include <vector>
#include "Game.h"
#include "DEFINITIONS.h"
#include "GeneticNN.h"
#include "GeneticAlgorithm.h"


int main() {
	std::vector<int> layers = { 2, 4, 5, 2 };

	std::vector<GeneticNN> networks = std::vector<GeneticNN>();
	Population<float> initPopulation = Population<float>();
	for (int i = 0; i < 10; i++) {
		networks.push_back(GeneticNN("Resources/neural_network/test" + std::to_string(i) + ".txt"));
		initPopulation.push_back(GeneticNN::ConvertConnectionNetworkToChromosome(&(networks.at(i))));
	}

	GeneticAlgorithm<float> ga(initPopulation, 0.7f, 0.09f);

	for (int i = 0; i < (int)networks.size(); i++) {
		networks[i].LoadConnectionNetworkFromChromosome(ga.GetPopulation().at(i));
	}

	std::vector<float> inputs = { 0.2f, 0.4f };

	for (GeneticNN& network : networks) {

		for (float out : network.Update(inputs)) {
			std::cout << out;
		}
	}

	system("pause");

	//Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}