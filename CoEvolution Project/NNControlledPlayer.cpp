#include "NNControlledPlayer.h"
#include <iostream>

NNControlledPlayer::NNControlledPlayer(GameDataRef data, Level ** level, sf::Vector2f wh, NeuralNetwork* networkController): Player::Player(data, level, wh, true), _networkController(networkController)
{
	this->_lives = 1;
}

NeuralNetwork * NNControlledPlayer::GetNetworkController()
{
	return this->_networkController;
}


bool NNControlledPlayer::Finished()
{
	if (!IsAlive() || GetProgress() >= 100.0f) {
		this->Deactivate();
		return true;
	}
	else {
		return false;
	}
}
