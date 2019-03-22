#include "TestNetworkState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>
#include <string>

TestNetworkState::TestNetworkState(GameDataRef data) :_data(data)
{
	this->_player = nullptr;
	this->_currentLevel = 0;
	this->_levels = std::vector<Level>();
}

void TestNetworkState::init()
{
	this->_levels = std::vector<Level>();

	//_levels.push_back(Level(Noise::GenHeightMap(sf::Vector2i(100, 4), 3, 2, 1), _data, "levelgentest-1", 15.0f));

	_levels.push_back(Level(_data, VALIDATION_LEVEL_1, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_2, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_3, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_4, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_5, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_6, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_7, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_8, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_9, LEVEL_1_TIME));
	_levels.push_back(Level(Noise::GenHeightMap(sf::Vector2i(100, 10), 8, 3, 1), _data, "levelgentest-1", 15.0f));
	
	std::string fileName;
	std::cout << "\n Enter name of the network file: ";
	std::cin >> fileName;
	this->_data->window.requestFocus();
	_player = new NNControlledPlayer(this->_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), new NeuralNetwork("Resources/" + fileName + ".net"));

	this->_data->gameObjectManager.addEntity(_player, PLAYER_LAYER);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.2f);
}

void TestNetworkState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
   	this->_levels.clear();
}

void TestNetworkState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->cleanup();
			this->_data->window.close();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.resize(event);
		}

		if (sf::Event::KeyPressed == event.type) {

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				this->_data->stateMachine.popState();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (_currentLevel + 1 < (int)this->_levels.size()) {
					this->_currentLevel++;
					this->_player->nextLevel();
					this->_player->restart();
				}
				else {
					this->_data->stateMachine.popState();
				}
			}

		}
	}
}

void TestNetworkState::update(float dt)
{
	if (_player != nullptr) {
		_player->getNetworkController()->run(_player->controllersViewOfLevel());
		std::vector<float> output = _player->getNetworkController()->getOutput();
		//given the outputs of the network 

		if (output.size() == 2) {
			//go left 
			if (output.at(0) > 0.9f) {
				_player->right();
			}
			else if (output.at(0) < -0.9f) {
				_player->left();
			}
			else {
				_player->stop();
			}
			// Jump
			if (output.at(1) > 0.9f) {
				_player->jump();
			}
			else {
				_player->stopJumping();
			}
		}
		if (_player->isFinished()) {
			if (_currentLevel + 1 < (int)this->_levels.size()) {
				this->_currentLevel++;
				this->_player->nextLevel();
				this->_player->restart();
			}
			else {
				this->_data->stateMachine.popState();
			}
		}
	}
	this->_data->gameObjectManager.update(dt);
}

void TestNetworkState::draw(float dt)
{
	this->_data->window.clear(sf::Color(234, 212, 170, 255));
	if (this->_player != nullptr) {
		this->_data->camera.update(_player->getSpriteCenterPosition(), sf::Vector2f(10,10));
	}
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
