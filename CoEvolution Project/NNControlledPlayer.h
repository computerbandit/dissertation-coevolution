#pragma once
#include "NeuralNetwork.h"
#include "Player.h"

class NNControlledPlayer : public Player {
public:

	NNControlledPlayer(GameDataRef data, Level** level, sf::Vector2f wh, NeuralNetwork* networkController);

	virtual void Die() override;
	virtual void Finish() override;

	//function to return a vector or inputs to the controlling network;
	void ParseDataToNNController();

	NeuralNetwork* GetNetworkController();
	void SetNNController(NeuralNetwork* network);
	bool Finished();

private:
	NeuralNetwork* _networkController;
};