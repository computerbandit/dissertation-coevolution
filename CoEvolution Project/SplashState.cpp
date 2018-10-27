#include <sstream>
#include <iostream>
#include "SplashState.h"
#include "MainMenuState.h"
#include "DEFINITIONS.h"

SplashState::SplashState(GameDataRef data) : _data(data)
{
}

void SplashState::Init()
{
	this->_data->assetManager.LoadTexture("Splash State Background", SPLASH_STATE_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.GetTexture("Splash State Background"));
	AssetManager::Rescale(_background, sf::Vector2f(this->_data->window.getSize()));
}

void SplashState::Cleanup()
{

}

void SplashState::HandleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			this->_data->stateMachine.PushState(StateRef(new MainMenuState(this->_data)));
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);
			AssetManager::Rescale(_background, sf::Vector2f(this->_data->window.getSize()));
		}
	}
}

void SplashState::Update(float dt)
{
	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME) {
		this->_data->stateMachine.PushState(StateRef(new MainMenuState(this->_data)));
	}
}

void SplashState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(_background);
	this->_data->window.display();
}
