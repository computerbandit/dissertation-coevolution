#pragma once
#include "State.h"
#include "Game.h"
#include "Level.h"
#include "NNControlledPlayer.h"
#include "NeuralNetworkGA.h"
#include <SFML/Graphics.hpp>

class TrainNetworkState : public State {
public:
	TrainNetworkState(GameDataRef data, float timetolive,  float speedMultiplier, bool display = false);
	// Inherited via State
	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void HandleEvents() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;


private:
	GameDataRef _data;
	NeuralNetworkGA _ga;
	Level * _level;
	std::vector<NNControlledPlayer> _playerPopulation;
	bool _display;
	sf::Clock _clock;
	float _ttl;

	//given a players distance from the final checkpoint what is the fitness of the network that is controlling
	bool EvaluateNNControlledPlayer(NNControlledPlayer& nnplayer);
};