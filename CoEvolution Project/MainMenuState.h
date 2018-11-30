#pragma once
#include "State.h"
#include "Game.h"
#include <vector>
#include <SFML/Graphics.hpp>

class MainMenuState : public State {
public:

	MainMenuState(GameDataRef data);
	// Inherited via State
	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void HandleEvents() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;
private:
	GameDataRef _data;

	sf::Sprite _background;
	//Main menu list of buttons that can be selected
	std::vector<sf::Sprite*> _buttons;

	sf::Sprite _playButton;
	sf::Sprite _trainButton;
	sf::Sprite _exitButton;
};