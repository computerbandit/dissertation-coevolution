#include "NNControlledPlayer.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, levels, currentLevel, wh), _networkController(networkController)
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
	//player has finished the level...
	this->_finished = true;
	//could start a clock and have the next level after like 2 secs;
	//and display you finished in overlay text or something;
	this->NextLevel();
	this->Restart();
}

void NNControlledPlayer::Restart()
{
	this->_lives = 1;
	this->_currentCheckpoint = 0;
	this->Respawn();
}

void NNControlledPlayer::NextLevel()
{
	if (_currentLevel + 1 < this->_levels.size()) {
		this->_currentLevel++;
	}
	else {
		std::cout << "Player has beaten the game, well done!\n" << std::endl;
		this->_data->stateMachine.PopState();
	}
	this->_currentCheckpoint = 0;
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
