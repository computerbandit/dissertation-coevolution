#include <iostream>
#include <vector>
#include "Game.h"
#include "DEFINITIONS.h"

int main() {
	srand(time(0));
	Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");

	game.run();
	return EXIT_SUCCESS;
}