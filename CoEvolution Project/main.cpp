#include <iostream>
#include "Game.h"
#include "DEFINITIONS.h"
#include "NeuralNetwork.h"
#include <vector>

int main() {
	std::vector<int> layers = { 4, 8, 6, 3 };
	NeuralNetwork network(layers);

	network.PrintNetwork();

	//Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}