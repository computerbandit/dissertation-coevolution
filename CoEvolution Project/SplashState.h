#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "State.h"

class SplashState :public State {
public:
	SplashState(GameDataRef data);

	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;
	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

private:
	GameDataRef _data;
	sf::Clock _clock;
	sf::Sprite _background;
};