#include "MainMenuState.h"
#include "TrainNetworkState.h"
#include "TestNetworkState.h"
#include "GameState.h"
#include "../Framework/DEFINITIONS.h"
#include "../Framework/Level.h"
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
	AssetManager::rescale(buttonSprite, sf::Vector2f(250.0f, 50.0f));

	this->_data->assetManager.loadFont("Menu Font", MENU_FONT_PATH);

	sf::Text text;
	text.setFont(this->_data->assetManager.getFont("Menu Font"));
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	_strings = std::vector<std::string>();
	_strings.push_back("Play");
	_strings.push_back("Train");
	_strings.push_back("Test");
	_strings.push_back("Generate");
	_strings.push_back("Exit");

	sf::Vector2f pos = sf::Vector2f(60.0f, 250.0f);
	int buttonNum = 0;

	for (std::string s : _strings) {
		text.setString(s);

		//set position of the button and text;


		int textwidth = text.getGlobalBounds().width;
		buttonSprite.setPosition(pos);

		text.setPosition(sf::Vector2f((buttonSprite.getGlobalBounds().width/2) - (textwidth/2) + (buttonSprite.getGlobalBounds().left), (buttonSprite.getGlobalBounds().top + buttonSprite.getGlobalBounds().height/2) - (text.getCharacterSize())));

		_buttons[s] = Button(buttonSprite, text);

		buttonNum++;
		if (buttonNum >= 4) {
			buttonNum = 0;
			pos.x += 425.0f;
			pos.y = 250.0f;
		}
		else {
			pos.y += buttonSprite.getGlobalBounds().height + 25;
		}
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
			
			if (this->_data->inputManager.isSpriteClicked(_buttons["Play"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.pushState(StateRef(new GameState(_data)), false);
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Train"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.pushState(StateRef(new TrainNetworkState(_data)),false);
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Generate"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				std::cout << "Generating Levels" << std::endl;
				std::cout << "--please wait--" << std::endl;
				for (int i = 0; i < 10; i++) {
					Level(Noise::GenHeightMap(sf::Vector2i(25, 10), 6, 3, 1), _data, (TRAINING_LEVEL_PATH "lvl-") + std::to_string(i), 15.0f);
				}
				std::cout << "DONE!" << std::endl;
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
