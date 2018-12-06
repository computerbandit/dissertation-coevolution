#pragma once
#include "NeuralNetwork.h"
#include "Player.h"

class NNControlledPlayer : public Player {
public:
	NNControlledPlayer(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh, NeuralNetwork* networkController);

	NeuralNetwork* GetNetworkController();
	void SetNNController(NeuralNetwork* network);
	
	virtual void Die();
	virtual void Finish();

	std::vector<float> ConrollersViewOfLevel(int tileDiameter) const;

	bool IsMakingProgress();
private:
	NeuralNetwork* _networkController;
	float _previousProgress = 0.0f;
};