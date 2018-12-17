#pragma once
#include "NeuralNetwork.h"
#include "Player.h"

class NNControlledPlayer : public Player {
public:
	NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController);

	NeuralNetwork* getNetworkController();
	void setNNController(NeuralNetwork* network);
	
	virtual void die();
	virtual void finish();

	std::vector<float> controllersViewOfLevel(int tileDiameter) const;

	bool isMakingProgress();
private:
	NeuralNetwork* _networkController;
	float _previousProgress = 0.0f;
};