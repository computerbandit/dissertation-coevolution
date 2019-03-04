#pragma once

#include "../Framework/State.h"
#include "../Framework/Game.h"
#include "../Framework/GUI/Button.h"
#include <map>
#include <SFML/Graphics.hpp>

class TrainingPauseState : public State {
public:

	TrainingPauseState(GameDataRef data);
	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;
	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;
private:
	GameDataRef _data;
	sf::Sprite _background;
	//Main menu list of buttons that can be selected
	std::map<std::string, Button> _buttons;
	std::vector<std::string> _strings;
	sf::Font _font;
};