#include "NNControlledPlayer.h"
#include "DEFINITIONS.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, levels, currentLevel, wh), _networkController(networkController)
{
}

NeuralNetwork * NNControlledPlayer::GetNetworkController()
{
	return this->_networkController;
}

void NNControlledPlayer::SetNNController(NeuralNetwork * network)
{
	this->_networkController = network;
}

void NNControlledPlayer::Die()
{
	if (!_finished) {
		_networkController->SetFitnessScore(GetProgress());
		this->_lives = 0;
		this->_previousProgress = 0.0f;
		this->Deactivate();
	}	
}

void NNControlledPlayer::Finish()
{
	this->Die();
	this->_finished = true;
}

//given the position and current level the the entity is currently in return a list of values regarding the solid state of the tiles around around the entity
std::vector<float> NNControlledPlayer::ConrollersViewOfLevel(int tileDiameter) const
{
	float diameter = TILE_SIZE * tileDiameter;
	sf::FloatRect view = sf::FloatRect(this->_position.x - (diameter /2), this->_position.y - (diameter / 2), diameter, diameter);
	std::vector<float> tileValues = std::vector<float>();
	//get to the pos of the entity in the grid position of the level
	for (Tile* t : this->_levels.at(this->_currentLevel).GetTilesInArea(view)) {
		float value = 0.0f;
		if (t->IsSolid()) {
			value = 1.0f;
		}
		else {
			value = 0.0f;
			if (t->GetTileID() == DEATH_TILE) {
				value = -1.0f;
			}
		}	
		tileValues.push_back(value);
	}
	return tileValues;
}

bool NNControlledPlayer::IsMakingProgress()
{
	if (this->GetProgress() > this->_previousProgress) {
		this->_previousProgress = this->GetProgress();
		return true;
	}
	else {
		return false;
	}
}
