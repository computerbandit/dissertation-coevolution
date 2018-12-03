#include "MainMenuState.h"
#include "TrainNetworkState.h"
#include "TestNetworkState.h"
#include "GameState.h"
#include "DEFINITIONS.h"
#include <iostream>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
}

void MainMenuState::Init()
{
	this->_data->camera = Camera(&(this->_data->window),this->_data->window.getSize(), sf::Vector2f(this->_data->window.getSize().x / 2.0f, this->_data->window.getSize().y / 2.0f));
	//Loading in assets for the main menu
	//Background
	this->_data->assetManager.LoadTexture("Main Menu Background", MAIN_MENU_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.GetTexture("Main Menu Background"));
	AssetManager::Rescale(_background, sf::Vector2f(this->_data->window.getSize()));
	//Menu Button
	this->_data->assetManager.LoadTexture("Menu Button", MENU_BUTTON_PATH);
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(this->_data->assetManager.GetTexture("Menu Button"));
	this->_data->assetManager.LoadFont("Menu Font", MENU_FONT_PATH);

	sf::Text text;
	text.setFont(this->_data->assetManager.GetFont("Menu Font"));
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

void MainMenuState::Cleanup()
{

}

void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}

		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);
			AssetManager::Rescale(_background, sf::Vector2f(this->_data->window.getSize()));
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			
			if (this->_data->inputManager.IsSpriteClicked(_buttons[_strings[0]]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.PushState(StateRef(new GameState(_data)), false);
			}
			else if (this->_data->inputManager.IsSpriteClicked(_buttons[_strings[1]]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.PushState(StateRef(new TrainNetworkState(_data, DEFUALT_TRAINNGNG_TIME_TO_LIVE, DEFUALT_TRAINNING_SPEED_MULTIPLIER, DISPLAY_TRAINNING)));
			}
			else if (this->_data->inputManager.IsSpriteClicked(_buttons["Test"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.PushState(StateRef(new TestNetworkState(this->_data)));
			}
			else if (this->_data->inputManager.IsSpriteClicked(_buttons[_strings[2]]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->window.close();
			}
		}
	}
}

void MainMenuState::Update(float dt)
{
}

void MainMenuState::Draw(float dt)
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
