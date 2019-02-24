#include "../States/GameState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>

GameState::GameState(GameDataRef data) : _data(data)
{
	//load camera, levels the player and add the player to the list of entities in the level.
	this->_coin = nullptr;
	this->_player = nullptr;
	this->_currentLevel = 0;
	this->_levels = std::vector<Level>();
}

void GameState::init()
{
	_levels.push_back(Level(Noise::GenHeightMap(sf::Vector2i(1000, 10), 9, 2, 1), _data, "levelgentest-1", 15.0f));
	//load the levels in to the level vector
	//_levels.push_back(Level(_data, TRAINING_LEVEL_1, LEVEL_1_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_2, LEVEL_2_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_3, LEVEL_3_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_4, LEVEL_4_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_5, LEVEL_5_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_6, LEVEL_6_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_7, LEVEL_7_TIME));
	
	//init entities
	_coin = new Coin(_data);
	_player = new Player(_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2));
	this->_data->gameObjectManager.addEntity(_player);
	this->_data->gameObjectManager.addEntity(_coin);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.4f);
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
	this->_data->camera.update(_player->getSpriteCenterPosition());
	this->_data->window.clear(sf::Color(234, 212, 170, 255));
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
