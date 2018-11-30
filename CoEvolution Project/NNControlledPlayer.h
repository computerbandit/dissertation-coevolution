#pragma once
#include "NeuralNetwork.h"
#include "Player.h"

class NNControlledPlayer : public Player {
public:

	NNControlledPlayer(GameDataRef data, Level** level, sf::Vector2f wh, NeuralNetwork* networkController);

	NeuralNetwork* GetNetworkController();
	bool Finished();

private:
	NeuralNetwork* _networkController;
};