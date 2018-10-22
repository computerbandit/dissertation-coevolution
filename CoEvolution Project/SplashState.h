#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "State.h"

class SplashState :public State {
public:
	SplashState(GameDataRef data);

	// Inherited via State
	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void HandleEvents() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

private:
	GameDataRef _data;
	sf::Clock _clock;
	sf::Sprite _background;
};