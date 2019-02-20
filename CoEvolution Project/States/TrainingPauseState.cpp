#include "TrainingPauseState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>

TrainingPauseState::TrainingPauseState(GameDataRef data) : _data(data)
{
}

void TrainingPauseState::init()
{
	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(this->_data->window.getSize().x / 2.0f, this->_data->window.getSize().y / 2.0f));
	//Loading in assets for the main menu
	_background.setTexture(this->_data->assetManager.getTexture("Main Menu Background"));
	AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
	//Menu Button
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(this->_data->assetManager.getTexture("Menu Button"));

	sf::Text text;
	text.setFont(this->_data->assetManager.getFont("Menu Font"));
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	_strings = std::vector<std::string>();
	_strings.push_back("Resume");
	_strings.push_back("Save Data");
	_strings.push_back("Save Best Network");
	_strings.push_back("Exit");

	sf::Vector2f pos((this->_data->window.getSize().x / 2) - buttonSprite.getLocalBounds().width / 2, 250.0f);


	for (std::string s : _strings) {
		text.setString(s);

		//set position of the button and text;
		buttonSprite.setPosition(pos);
		text.setPosition(sf::Vector2f(pos.x + (buttonSprite.getLocalBounds().width / 4), pos.y + (buttonSprite.getLocalBounds().height / 4)));
		pos.y += buttonSprite.getLocalBounds().height + 25;
		_buttons[s] = Button(buttonSprite, text);
	}
}

void TrainingPauseState::cleanup()
{

}

void TrainingPauseState::handleEvents()
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

			if (this->_data->inputManager.isSpriteClicked(_buttons["Resume"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				this->_data->stateMachine.popState();
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Save Data"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				std::cout << "Save data function need to be triggered" << std::endl;
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Save Best Network"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				std::cout << "Save best network function need to be triggered" << std::endl;
			}
			else if (this->_data->inputManager.isSpriteClicked(_buttons["Exit"]._sprite, sf::Mouse::Button::Left, this->_data->window))
			{
				//need to tell the main state to exit somehow
				this->_data->stateMachine.popState();
				this->_data->stateMachine.setSTF(EXIT);
			}
		}
	}
}

void TrainingPauseState::update(float dt)
{
}

void TrainingPauseState::draw(float dt)
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
