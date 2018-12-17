#include "NNControlledPlayer.h"
#include "DEFINITIONS.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, levels, currentLevel, wh), _networkController(networkController)
{
}

NeuralNetwork * NNControlledPlayer::getNetworkController()
{
	return this->_networkController;
}

void NNControlledPlayer::setNNController(NeuralNetwork * network)
{
	this->_networkController = network;
}

void NNControlledPlayer::die()
{
	if (!_finished) {
		_networkController->setFitnessScore(getProgress());
		this->_lives = 0;
		this->_previousProgress = 0.0f;
		this->deactivate();
	}	
}

void NNControlledPlayer::finish()
{
	this->die();
	this->_finished = true;
}

//given the position and current level the the entity is currently in return a list of values regarding the solid state of the tiles around around the entity
std::vector<float> NNControlledPlayer::controllersViewOfLevel(int tileDiameter) const
{
	float diameter = TILE_SIZE * tileDiameter;
	sf::FloatRect view = sf::FloatRect(this->_position.x - (diameter /2), this->_position.y - (diameter / 2), diameter, diameter);
	std::vector<float> tileValues = std::vector<float>();
	//get to the pos of the entity in the grid position of the level
	for (Tile* t : this->_levels.at(this->_currentLevel).getTilesInArea(view)) {
		float value = 0.0f;
		if (t->isSolid()) {
			value = 1.0f;
		}
		else {
			value = 0.0f;
			if (t->getTileID() == DEATH_TILE) {
				value = -1.0f;
			}
		}	
		tileValues.push_back(value);
	}
	return tileValues;
}

bool NNControlledPlayer::isMakingProgress()
{
	if (this->getProgress() > this->_previousProgress) {
		this->_previousProgress = this->getProgress();
		return true;
	}
	else {
		return false;
	}
}
