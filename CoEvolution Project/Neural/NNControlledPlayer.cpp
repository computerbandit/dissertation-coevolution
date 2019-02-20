#include "NNControlledPlayer.h"
#include "../Framework/DEFINITIONS.h"
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
		//_networkController->setFitnessScore(getProgress());
		_networkController->setFitnessScore(0.0f);
	}
	else { //if the player has finished then the controller gets loads more fitness points
		float leveltime = this->_timer.getElapsedTime().asSeconds();
		float timepoints = ((leveltime * this->_data->gameSpeedMultiplier) - this->_levels->at(this->_currentLevel).getLevelTime()) * 2;
		_networkController->setFitnessScore(1000.0f);
		//_networkController->setFitnessScore(getProgress());
		//_networkController->setFitnessScore(getProgress() + 500.0f);
		//_networkController->setFitnessScore(getProgress() + 1000.0f + timepoints);
		//_networkController->setFitnessScore((getProgress() + 500.0f + timepoints ) * (1.0f + ((float)this->_currentLevel/10)));
	}
	this->_lives = 0;
	this->_previousProgress = 0.0f;
	this->deactivate();
}

void NNControlledPlayer::finish()
{
	this->_finished = true;
	this->die();
}

//given the position and current level the the entity is currently in return a list of values regarding the solid state of the tiles around around the entity
std::vector<float> NNControlledPlayer::controllersViewOfLevel(int up, int down, int left, int right) const
{
	int x_tile = int(this->getSpriteCenterPosition().x / TILE_SIZE) * TILE_SIZE;
	int y_tile = int(this->getSpriteCenterPosition().y / TILE_SIZE) * TILE_SIZE;

	sf::FloatRect view = sf::FloatRect(x_tile - (left*TILE_SIZE), y_tile - (up*TILE_SIZE), ((right+left+1)*TILE_SIZE) - TILE_SIZE / 10, ((up+down+1)*TILE_SIZE) - TILE_SIZE/10);
	std::vector<float> tileValues = std::vector<float>();
	//get to the pos of the entity in the grid position of the level
	std::vector<Tile*> tilesInArea = this->_levels->at(this->_currentLevel).getTilesInArea(view);
	float value = 0.0f;
	//assign the value of the tile to a number for the controllers perception
	for (int i = 0; i < (int)tilesInArea.size(); i++) {
		if (tilesInArea.at(i)->isSolid()) {
			value = 100.0f;
		}
		else {
			switch (tilesInArea.at(i)->getTileID())
			{
			case DEATH_TILE:
				value = -100.0f;
				break;
			case CHECKPOINT_TILE:
				value = 0.0f;
				break;
			case FINISH_LINE_TILE:
				value = 0.0f;
				break;
			default:
				value = 0.0f;
				break;
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
