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
	std::vector<Level> _levels;
	int _currentLevel = 0;

	NeuralNetworkGA _ga;
	std::vector<NNControlledPlayer> _playerPopulation;
	sf::Clock _ttlClock;
	sf::Clock _checkProgressClock;
	bool _display;
	float _ttl;
	sf::Text _info;
	std::string _token;
};