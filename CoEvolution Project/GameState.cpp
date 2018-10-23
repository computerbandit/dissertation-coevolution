#include "GameState.h"
#include "Player.h"
#include "Level.h"
#include "DEFINITIONS.h"
#include <iostream>

Player* player;
Level* level;

GameState::GameState(GameDataRef data) : _data(data)
{
	//init camera, level the player and add the player to the list of entities in the level.
	level = new Level(_data);
	this->_data->camera = Camera(&(this->_data->window), (float)this->_data->window.getSize().x, (float)(float)this->_data->window.getSize().y);
	player = new Player(_data, sf::Vector2f(0, 0));
	this->_data->gameObjectManager.AddEntity(player);

}

void GameState::Init()
{
	//load backgroung in
	this->_data->assetManager.LoadTexture("Background Test", BACKGROUND_TEST);
	this->_background.setTexture(this->_data->assetManager.GetTexture("Background Test"));
	
	//load tile sprites
	this->_data->assetManager.LoadTexture("Grass_Tile", GRASS_TILE);
	
	//load the level text file and set up tiles.
	level->LoadLevelFromTextFile(TEST_LEVEL);
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
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(_background);
	level->Draw();
	this->_data->gameObjectManager.Draw(dt);
	this->_data->window.display();
}
