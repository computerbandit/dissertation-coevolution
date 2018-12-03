#pragma once
#include "State.h"
#include "Game.h"
#include <map>
#include <SFML/Graphics.hpp>

struct Button {
	sf::Sprite _sprite;
	sf::Text _text;
	Button() {};
	Button(sf::Sprite sprite, sf::Text text): _sprite(sprite), _text(text) {}
};

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
	std::map<std::string, Button> _buttons;
	std::vector<std::string> _strings;
	sf::Font _font;
};