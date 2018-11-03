#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>

int main() {

	std::vector<int> layers = { 2, 2, 1 };
	NeuralNetwork network(std::string("Resources/neural_network/test.txt"));

	std::cout << network.ToString() << std::endl;
	//network.SaveNetwork();


	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;

	
}