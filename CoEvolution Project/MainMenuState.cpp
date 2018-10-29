#include "MainMenuState.h"
#include "GameState.h"
#include "DEFINITIONS.h"
#include <iostream>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
}

void MainMenuState::Init()
{
	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(this->_data->window.getSize().x / 2, this->_data->window.getSize().y / 2));
	//Loading in assets for the main menu
	//Background
	this->_data->assetManager.LoadTexture("Main Menu Background", MAIN_MENU_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.GetTexture("Main Menu Background"));
	AssetManager::Rescale(_background, sf::Vector2f(this->_data->window.getSize()));
	//Play Button
	this->_data->assetManager.LoadTexture("Main Menu Play Button", MAIN_MENU_PLAY_BUTTON_PATH);
	_playButton.setTexture(this->_data->assetManager.GetTexture("Main Menu Play Button"));
	//Exit Button 
	this->_data->assetManager.LoadTexture("Main Menu Exit Button", MAIN_MENU_EXIT_BUTTON_PATH);
	_exitButton.setTexture(this->_data->assetManager.GetTexture("Main Menu Exit Button"));

	//push the references of the loaded sprites onto the button vector
	_buttons.push_back(&_playButton);
	_buttons.push_back(&_exitButton);

	//change the position of the buttons 
	_playButton.setPosition(SCREEN_WIDTH / 2.0f - _playButton.getGlobalBounds().width / 2.0f, (int)_playButton.getGlobalBounds().height * 3.0f);
	_exitButton.setPosition(SCREEN_WIDTH / 2.0f - _exitButton.getGlobalBounds().width / 2.0f, (int)_exitButton.getGlobalBounds().height * 4.5f);

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
			for (sf::Uint16 i = 0; i < _buttons.size(); i++) {
				if (this->_data->inputManager.IsSpriteClicked(*(_buttons[i]), sf::Mouse::Button::Left, this->_data->window)) {
					switch (i)
					{

					case 0:
						//Play Button action -> move to the play game state
						this->_data->stateMachine.PushState(StateRef(new GameState(_data)));
						break;
					case 1:
						//Help - this is like what the user needs to know and how to play the game 
						this->_data->window.close();
						break;
					case 2:
						//Exit Button action -> Close the game
						this->_data->window.close();
						break;
					default:
						break;
					}

					break;
				}
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

	for (sf::Sprite* button : _buttons) {
		this->_data->window.draw(*button);
	}

	this->_data->window.display();
}
