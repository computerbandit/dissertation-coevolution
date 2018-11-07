#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>

int main() {

	std::vector<int> layers = { 4, 6, 8, 3 };
	NeuralNetwork network(layers, std::string("Resources/neural_network/test.txt"));
	std::cout << network.ToString();
	std::vector<float> input = {0.5f, 0.2f, 0.8f, 0.1f};
	//testing the neural network 10 passes
	for (int i = 0; i < 1; i++) {
		std::vector<float> out = network.Update(input, true);
		for (float f : out) {
			std::cout << f << " ";
		}
		std::cout << "\n";
	}


	system("pause");

	//Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}