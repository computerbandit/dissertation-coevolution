#pragma once
#include "NeuralNetwork.h"
#include "Player.h"

class NNControlledPlayer : public Player {
public:


	NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController);


	//function to return a vector or inputs to the controlling network;
	void ParseDataToNNController();

	NeuralNetwork* GetNetworkController();
	void SetNNController(NeuralNetwork* network);
	bool Finished();

	std::vector<float> ConrollersViewOfLevel(int tileDiameter) const;

private:
	NeuralNetwork* _networkController;
	const int _startingLives = 1;
};