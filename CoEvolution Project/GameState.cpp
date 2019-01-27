#include "GameState.h"
#include "DEFINITIONS.h"
#include <iostream>

GameState::GameState(GameDataRef data) : _data(data)
{
	//load camera, levels the player and add the player to the list of entities in the level.
	this->_levels = std::vector<Level>();
}

void GameState::init()
{
	//load the levels in to the level vector
	_levels.push_back(Level(_data, TRAINNING_LEVEL_3));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));

	//init Player
	_player = new Player(_data, &_levels, sf::Vector2f(16, 16));
	this->_data->gameObjectManager.addEntity(_player);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
}

void GameState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_levels.clear();
	delete this->_player;
}

void GameState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.resize(event);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			this->_data->stateMachine.popState();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			_player->left();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			_player->right();
		}
		else {
			_player->stop();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			_player->jump();
		}

		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Space) {
				_player->stopJumping();
			}
		}
	}
}

void GameState::update(float dt)
{
	this->_data->gameObjectManager.update(dt);

	if (_player->isFinished()) {
		if (_currentLevel + 1 < (int)this->_levels.size()) {

			std::cout << "\n Level Completed" << std::endl;
			this->_currentLevel++;
			this->_player->nextLevel();
			this->_player->restart();
		}
		else {
			std::cout << "Well Done you finished" << std::endl;
			this->_data->stateMachine.popState();
		}
	}
}

void GameState::draw(float dt)
{
	this->_data->camera.update(_player->getPosition());
	this->_data->window.clear(sf::Color::White);
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
