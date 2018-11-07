#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>


int main() {

	std::vector<int> layers1 = { 4, 6, 8, 3 };
	std::vector<int> layers2 = { 1, 2, 1};

	NeuralNetwork network1(layers2, std::string("Resources/neural_network/test1.txt"));

	std::cout << network1.ToString();

	std::vector<float> input = {0.5f};
	//testing the neural network 10 passes
	std::cout << "Output: " << std::endl;
	for (int i = 0; i < 1; i++) {
		std::vector<float> out = network1.Update(input);
		for (float f : out) {
			std::cout << f << " ";
		}
		std::cout << "\n";
	}

	system("pause");

	//Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}