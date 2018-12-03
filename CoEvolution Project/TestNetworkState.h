#pragma once
#include "State.h"
#include "Game.h"
#include "Level.h"
#include "NNControlledPlayer.h"
#include <SFML/Graphics.hpp>

class TestNetworkState : public State {
public:
	TestNetworkState(GameDataRef data);
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

	NNControlledPlayer * _player;
};