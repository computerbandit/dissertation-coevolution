#include "TestNetworkState.h"
#include "DEFINITIONS.h"
#include <iostream>
#include <string>

TestNetworkState::TestNetworkState(GameDataRef data) :_data(data)
{
	 _levels = std::vector<Level>();
}

void TestNetworkState::init()
{

	//load the levels in the order to play them;
	
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_3));
	std::string fileName;
	std::cout << "\n Enter name of the network file: ";
	std::cin >> fileName;
	this->_data->window.requestFocus();
	_player = new NNControlledPlayer(this->_data, &_levels, sf::Vector2f(16, 16), new NeuralNetwork("Resources/networks/"+ fileName + ".txt"));

	this->_data->gameObjectManager.addEntity(_player);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
}

void TestNetworkState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				this->_data->gameSpeedMultiplier = 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				this->_data->gameSpeedMultiplier = 2.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				this->_data->gameSpeedMultiplier = 3.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				this->_data->gameSpeedMultiplier = 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				this->_data->gameSpeedMultiplier = 5.0f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				this->_data->stateMachine.popState();
			}

		}
	}
}

void TestNetworkState::update(float dt)
{
	_player->getNetworkController()->run(_player->controllersViewOfLevel(2, 2, 2, 4));
	std::vector<float> output = _player->getNetworkController()->getOutput();
	//given the outputs of the network 

	// go left
	if (output.at(0) > 0.9f) {
		_player->right();
	}
	// go right
	if (output.at(1) > 0.9f) {
		_player->left();
	}
	// Jump
	if (output.at(2) > 0.9f) {
		_player->jump();
	}
	else {
		_player->stopJumping();
	} 

	this->_data->gameObjectManager.update(dt);
	this->_data->camera.update(_player->getPosition());
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

void TestNetworkState::draw(float dt)
{
	this->_data->window.clear(sf::Color::White);
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
