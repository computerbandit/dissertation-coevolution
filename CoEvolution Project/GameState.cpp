#include "GameState.h"
#include "Player.h"
#include "Level.h"
#include "DEFINITIONS.h"
#include <iostream>

Player* player;

GameState::GameState(GameDataRef data) : _data(data)
{
	//init camera, level the player and add the player to the list of entities in the level.
	this->_level = new Level(_data);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
}

void GameState::Init()
{
	//load tile sprites
	this->_data->assetManager.LoadTexture(GRASS_TILE_TEX, GRASS_TILE_TEX_PATH);
	this->_data->assetManager.LoadTexture(CHECKPOINT_TILE_TEX, CHECKPOINT_TILE_TEX_PATH);
	this->_data->assetManager.LoadTexture(FINISH_LINE_TILE_TEX, FINISH_LINE_TILE_TEX_PATH);
	//load the level text file and set up tiles.
	this->_level->LoadLevel(1);

	//init Player
	this->_data->assetManager.LoadTexture("Player_Sprite", PLAYER_TEX_PATH);
	player = new Player(_data, &_level, sf::FloatRect(0, 0, 30, 60));
	this->_data->gameObjectManager.AddEntity(player);


}

void GameState::Cleanup()
{
	this->_data->camera.Restore();
}

void GameState::HandleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
			this->Cleanup();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			player->Left();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			player->Right();
		}
		else {
			player->Stop();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player->Jump();
		}
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Space) {
				player->_holdingJump = false;
			}
		}
	}
}

void GameState::Update(float dt)
{
	this->_data->gameObjectManager.Update(dt);
	this->_data->camera.Update(player->GetPosition());
}

void GameState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::White);
	this->_level->Draw();
	this->_data->gameObjectManager.Draw(dt);
	this->_data->window.display();
}
