#include "MainMenuState.h"
#include "TrainNetworkState.h"
#include "TestNetworkState.h"
#include "GameState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
}

void MainMenuState::init()
{
	this->_data->camera = Camera(&(this->_data->window),this->_data->window.getSize(), sf::Vector2f(this->_data->window.getSize().x / 2.0f, this->_data->window.getSize().y / 2.0f));
	//Loading in assets for the main menu
	//Background
	this->_data->assetManager.loadTexture("Main Menu Background", MAIN_MENU_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.getTexture("Main Menu Background"));
	AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
	//Menu Button
	this->_data->assetManager.loadTexture("Menu Button", MENU_BUTTON_PATH);
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(this->_data->assetManager.getTexture("Menu Button"));
	this->_data->assetManager.loadFont("Menu Font", MENU_FONT_PATH);

	sf::Text text;
	text.setFont(this->_data->assetManager.getFont("Menu Font"));
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	_strings = std::vector<std::string>();
	_strings.push_back("Play");
	_strings.push_back("Train");
	_strings.push_back("Test");
	_strings.push_back("Exit");

	sf::Vector2f pos((this->_data->window.getSize().x / 2) - buttonSprite.getLocalBounds().width/2, 250.0f);


	for (std::string s : _strings) {
		text.setString(s);

		//set position of the button and text;
		buttonSprite.setPosition(pos);
		text.setPosition(sf::Vector2f(pos.x + (buttonSprite.getLocalBounds().width / 4), pos.y + (buttonSprite.getLocalBounds().height / 4)));
		pos.y += buttonSprite.getLocalBounds().height + 25;
		_buttons[s] = Button(buttonSprite, text);
	}
}

void MainMenuState::cleanup()
{

}

void MainMenuState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}

		if (sf::Event::Resized == event.type) {
			this->_data->camera.resize(event);
			//AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			
			if (this->_data->inputManager.isSpriteClicked(_buttons[_strings[0]]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.pushState(StateRef(new GameState(_data)), false);
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons[_strings[1]]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.pushState(StateRef(new TrainNetworkState(_data)),false);
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Test"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.pushState(StateRef(new TestNetworkState(this->_data)), false);
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Exit"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->window.close();
			}
		}
	}
}

void MainMenuState::update(float dt)
{
}

void MainMenuState::draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	//draw background
	this->_data->window.draw(_background);
	//draw buttons

	for (std::string s : _strings) {
		this->_data->window.draw(_buttons[s]._sprite);
		this->_data->window.draw(_buttons[s]._text);
	}

	this->_data->window.display();
}
