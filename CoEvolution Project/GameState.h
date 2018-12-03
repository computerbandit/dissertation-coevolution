#pragma once
#include "State.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

class GameState : public State {
public:
	GameState(GameDataRef data);
	// Inherited via State
	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void HandleEvents() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;
private:
	GameDataRef _data;
	std::vector<Level> _levels;
	int _currentLevel;
	Player * _player;
};