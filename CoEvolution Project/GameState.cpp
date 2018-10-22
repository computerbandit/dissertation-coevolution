#include "GameState.h"
#include "Player.h"
#include "Level.h"
#include "DEFINITIONS.h"
#include <iostream>

Player* player;
Level* level;

GameState::GameState(GameDataRef data) : _data(data)
{
	player = new Player(_data, sf::Vector2f(0, 0));
	this->_data->entityManager.Add(player);
	this->_data->camera.Init(&(this->_data->window), (float)(this->_data->window.getSize().x), (float)(this->_data->window.getSize().y));

	level = new Level(_data);

}

void GameState::Init()
{
	//load backgroung in
	this->_data->assetManager.LoadTexture("Background Test", BACKGROUND_TEST);
	this->_background.setTexture(this->_data->assetManager.GetTexture("Background Test"));

	if (level->LoadLevelFromTextFile(TEST_LEVEL)) {
		level->Init();
	}
	else {
		std::cout << "map failed to load" << std::endl;
	}

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
	this->_data->entityManager.Update(dt);
	this->_data->camera.Update(player->GetPosition());
}

void GameState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(_background);
	level->Draw(dt);
	this->_data->entityManager.Draw(dt);
	this->_data->window.display();
}
