#pragma once
#include "NeuralNetwork.h"
#include "../Entity/Player.h"

class NNControlledPlayer : public Player {
public:
	NNControlledPlayer(GameDataRef data, std::vector<Level>* levels, sf::Vector2f wh, NeuralNetwork* networkController, int up, int down, int left, int right);

	NeuralNetwork* getNetworkController();
	void setNNController(NeuralNetwork* network);
	
	virtual void die();
	virtual void finish();

	std::vector<float> controllersViewOfLevel() const;

	bool isMakingProgress();
private:
	NeuralNetwork* _networkController;
	float _previousProgress = 0.0f;
	int _up, _down, _left, _right;
};