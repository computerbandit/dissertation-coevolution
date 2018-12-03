#include "GameState.h"
#include "DEFINITIONS.h"
#include <iostream>

GameState::GameState(GameDataRef data) : _data(data)
{
	//load camera, levels the player and add the player to the list of entities in the level.
	this->_levels = std::vector<Level>();
}

void GameState::Init()
{
	//load the texturesheet
	this->_data->assetManager.LoadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));

	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));

	//init Player
	this->_data->assetManager.LoadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(16, 16));
	_player = new Player(_data, _levels, _currentLevel, sf::Vector2f(16, 16));
	this->_data->gameObjectManager.AddEntity(_player);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
}

void GameState::Cleanup()
{
	this->_data->camera.Restore();
	this->_data->gameObjectManager.ClearEntities();
	delete this->_player;
}

void GameState::HandleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			_player->Left();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			_player->Right();
		}
		else {
			_player->Stop();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			_player->Jump();
		}
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Space) {
				_player->StopJumping();
			}
		}
	}
}

void GameState::Update(float dt)
{
	this->_data->gameObjectManager.Update(dt);
}

void GameState::Draw(float dt)
{
	this->_data->camera.Update(_player->GetPosition());
	this->_data->window.clear(sf::Color::White);
	this->_levels.at(this->_currentLevel).Draw();
	this->_data->gameObjectManager.Draw(dt);
	this->_data->window.display();
}
