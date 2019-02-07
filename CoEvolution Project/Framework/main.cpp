#include <iostream>
#include <vector>
#include "Game.h"
#include "DEFINITIONS.h"

int main() {
	srand(time(0));
	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "CoEvil Prototype");
	return EXIT_SUCCESS;
}