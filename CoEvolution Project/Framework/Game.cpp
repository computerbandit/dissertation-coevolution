#include "Game.h"
#include "../States/SplashState.h"
#include "DEFINITIONS.h"
#include <string>

Game::Game(int width, int height, std::string title)
{
	this->_data->window.create(sf::VideoMode(width, height), title);
	this->_data->window.setVerticalSyncEnabled(false);
	this->_data->stateMachine.pushState(StateRef(new SplashState(this->_data)));
	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(this->_data->window.getSize().x/2, this->_data->window.getSize().y/2));
	this->_data->window.setFramerateLimit(0);
}

void Game::run()
{
	float newTime, frameTime, interpolation;
	float currentTime = this->_clock.getElapsedTime().asSeconds();
	float accumulator = 0.0f;

	while (this->_data->window.isOpen()) {
		this->_data->stateMachine.processStateChanges();
		newTime = this->_clock.getElapsedTime().asSeconds();

		frameTime = newTime - currentTime;
		if (frameTime > 0.25f) {
			frameTime = 0.25f;
		}
		currentTime = newTime;

		this->_data->stateMachine.getAvtiveState()->handleEvents();
		if (!this->_data->releaseAccumulator) {
			accumulator += frameTime;
			while (accumulator >= dt) {
				this->_data->stateMachine.getAvtiveState()->update(dt);
				accumulator -= dt;
			}
		}
		else
		{
			this->_data->stateMachine.getAvtiveState()->update(dt);
		}
		interpolation = accumulator / dt;
		this->_data->stateMachine.getAvtiveState()->draw(interpolation);

		
	}
}
