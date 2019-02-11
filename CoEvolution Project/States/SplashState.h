#pragma once

#include <SFML/Graphics.hpp>
#include "../Framework/Game.h"
#include "../Framework/State.h"

class SplashState :public State {
public:
	SplashState(GameDataRef data);

	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;
	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

	void loadingPhase();

private:
	GameDataRef _data;
	sf::Clock _clock;
	sf::Sprite _background;

	bool _loadAssets;
};