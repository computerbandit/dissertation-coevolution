#include "NNControlledPlayer.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, Level ** level, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, level, wh), _networkController(networkController)
{
}

void NNControlledPlayer::Die()
{
	_lives = 0;
	this->SetProgress(this->PercentageOfLevelCompleted());
	this->Deactivate();
}

void NNControlledPlayer::Finish()
{
	this->Die();
	//(*_level)->LoadNextLevel();
	//this->Respawn();
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
