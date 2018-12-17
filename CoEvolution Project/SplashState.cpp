#include <sstream>
#include <iostream>
#include "SplashState.h"
#include "MainMenuState.h"
#include "DEFINITIONS.h"

SplashState::SplashState(GameDataRef data) : _data(data)
{
}

void SplashState::init()
{
	this->_data->assetManager.loadTexture("Splash State Background", SPLASH_STATE_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.getTexture("Splash State Background"));
	AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
}

void SplashState::cleanup()
{

}

void SplashState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			this->_data->stateMachine.pushState(StateRef(new MainMenuState(this->_data)));
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.resize(event);
			AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
		}
	}
}

void SplashState::update(float dt)
{
	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME) {
		this->_data->stateMachine.pushState(StateRef(new MainMenuState(this->_data)));
	}
}

void SplashState::draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(_background);
	this->_data->window.display();
}
