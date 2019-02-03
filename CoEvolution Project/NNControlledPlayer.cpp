#include "NNControlledPlayer.h"
#include "DEFINITIONS.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, std::vector<Level>* levels, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, levels, wh), _networkController(networkController)
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
std::vector<float> NNControlledPlayer::controllersViewOfLevel(int up, int down, int left, int right) const
{
	int x_tile = int(this->_position.x / TILE_SIZE) * TILE_SIZE;
	int y_tile = int(this->_position.y / TILE_SIZE) * TILE_SIZE;

	sf::FloatRect view = sf::FloatRect(x_tile - (left*TILE_SIZE), y_tile - (up*TILE_SIZE), ((right+left+1)*TILE_SIZE) - TILE_SIZE / 10, ((up+down+1)*TILE_SIZE) - TILE_SIZE/10);
	std::vector<float> tileValues = std::vector<float>();
	//get to the pos of the entity in the grid position of the level
	std::vector<Tile*> tilesInArea = this->_levels->at(this->_currentLevel).getTilesInArea(view);
	float value = 0.0f;
	for (int i = 0; i < (int)tilesInArea.size(); i++) {
		if (tilesInArea.at(i)->isSolid()) {
			value = 5.0f;
		}
		else {
			if (tilesInArea.at(i)->getTileID() == DEATH_TILE) {
				value = -15.0f;
			}
			else {
				value = -5.0f;
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
