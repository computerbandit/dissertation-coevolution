#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>

int main() {

	std::vector<int> layers = { 2, 6, 8, 3 };
	NeuralNetwork network(layers, std::string("Resources/neural_network/test.txt"));

	std::cout << network.ToString() << std::endl;
	//network.SaveNetwork();
	std::vector<float> inputs = { 0.2f, 0.2f };
	network.Update(inputs);


	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;

	
}