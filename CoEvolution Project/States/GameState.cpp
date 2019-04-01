#include "../States/GameState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>

GameState::GameState(GameDataRef data) : _data(data)
{
	//load camera, levels the player and add the player to the list of entities in the level.
	this->_player = nullptr;
	this->_currentLevel = 0;
	this->_levels = std::vector<Level>();
}

void GameState::init()
{
	this->_levels = std::vector<Level>();
	//load the levels in to the level vector


	Level A = Level(Level(Noise::GenHeightMap(sf::Vector2i(10, 9), 8, 2, 1), _data, GAME_LEVEL_PATH"lvl-1", 15.0f), Level(Noise::GenHeightMap(sf::Vector2i(10, 9), 8, 2, 1), _data, GAME_LEVEL_PATH"lvl-2", 15.0f), "templevel");
	Level B = Level(A, Level(Noise::GenHeightMap(sf::Vector2i(10, 4), 3, 2, 1), _data, GAME_LEVEL_PATH"lvl-2", 15.0f), "templevel");
	Level C = Level(A, B, "templevel");
	_levels.clear();
	_levels.push_back(C);
	
	//init entities
	_player = new Player(_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2));
	this->_data->gameObjectManager.clearEntities();
	this->_data->gameObjectManager.addEntity(_player, PLAYER_LAYER);

	//load the entites for the first level;
	this->_levels.at(_currentLevel).spawnEntities();


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
		switch (event.type)
		{
		case sf::Event::Closed:
			this->_data->window.close();
			break;
		case sf::Event::Resized:
			this->_data->camera.resize(event);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::A:
				_player->left();
				break;
			case sf::Keyboard::D:
				_player->right();
				break;
			case sf::Keyboard::Space:
				_player->jump();
				break;
			case sf::Keyboard::Escape:
				this->_data->stateMachine.popState();
				break;
			}
			break;
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::D:
					_player->stop();
					break;
				case sf::Keyboard::Space:
					_player->stopJumping();
					break;
				}
			break;
		}
	}
}

void GameState::update(float dt)
{
	this->_data->gameObjectManager.update(dt);

	if (_player->isFinished()) {
		Level nextlvl = Level(Noise::GenHeightMap(sf::Vector2i(100, 10), 9, 2, 1), _data, GAME_LEVEL_PATH"lvl-1", 15.0f);
		this->_levels.push_back(nextlvl);
		if (_currentLevel + 1 < (int)this->_levels.size()) {

			std::cout << "\n Level Completed" << std::endl;
			this->_currentLevel++;
			this->_data->gameObjectManager.clearEntities();
			this->_data->gameObjectManager.addEntity(_player, PLAYER_LAYER);
			//load the entites for the first level;

			this->_levels.at(this->_currentLevel).spawnEntities();

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
	this->_data->camera.update(_player->getSpriteCenterPosition(), sf::Vector2f(10,10));
	this->_data->window.clear(sf::Color(99, 155, 255, 255));
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
