#pragma once
#include "../Framework/State.h"
#include "../Framework/Game.h"
#include "../Framework/Level.h"
#include "../Entity/Player.h"
#include <SFML/Graphics.hpp>

class GameState : public State {
public:
	GameState(GameDataRef data);
	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;
	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;
private:
	GameDataRef _data;
	std::vector<Level> _levels;
	int _currentLevel;
	Player * _player;
};