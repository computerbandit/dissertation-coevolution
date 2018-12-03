#include "NNControlledPlayer.h"
#include "DEFINITIONS.h";
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, levels, currentLevel, wh), _networkController(networkController)
{
}

void NNControlledPlayer::ParseDataToNNController()
{
	//this is the data that we will parse to the controlling network so that it can run a pass on the layers and get an output
	std::vector<float> data = std::vector<float>();
}

NeuralNetwork * NNControlledPlayer::GetNetworkController()
{
	return this->_networkController;
}

void NNControlledPlayer::SetNNController(NeuralNetwork * network)
{
	this->_networkController = network;
}


bool NNControlledPlayer::Finished()
{
	if (!IsAlive() || GetProgress() >= 100.0f) {
		
		return true;
	}
	else {
		return false;
	}
}

//given the position and current level the the entity is currently in return a list of values regarding the solid state of the tiles around around the entity
std::vector<float> NNControlledPlayer::ConrollersViewOfLevel(int tileDiameter) const
{
	float diameter = TILE_SIZE * tileDiameter;
	sf::FloatRect view = sf::FloatRect(this->_position.x - (diameter /2), this->_position.y - (diameter / 2), diameter, diameter);
	std::vector<float> tileSolidState = std::vector<float>();
	//get to the pos of the entity in the grid position of the level
	for (Tile* t : this->_levels.at(this->_currentLevel).GetTilesInArea(view)) {
		tileSolidState.push_back((t->IsSolid())? 1.0f : 0.0f);
	}
	return tileSolidState;
}
