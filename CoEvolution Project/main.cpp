#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>

int main() {

	std::vector<int> layers = { 1, 2, 3, 4 };
	NeuralNetwork network(layers, std::string("Resources/nns/test.txt"));

	std::cout << network.ToString() << std::endl;

	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}