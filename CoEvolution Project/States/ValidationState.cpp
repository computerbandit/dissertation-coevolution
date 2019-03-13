#include "ValidationState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>
#include <fstream>
#include <string>

ValidationState::ValidationState(GameDataRef data, std::string token, int popSize) :_data(data), _token(token), _populationSize(popSize)
{
	this->_ttl = 100.0f;
	this->_currentLevel = 0;
	this->_levels = std::vector<Level>();

	
}

void ValidationState::init()
{
	this->_levels = std::vector<Level>();

	_levels.push_back(Level(_data, VALIDATION_LEVEL_1, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_2, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_3, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_4, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_5, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_6, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_7, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_8, LEVEL_1_TIME));
	_levels.push_back(Level(_data, VALIDATION_LEVEL_9, LEVEL_1_TIME));

	std::ofstream file;
	//load in the population
	for (int i = 0; i < _populationSize; i++) {
		this->_population.push_back(NNControlledPlayer(this->_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), new NeuralNetwork("Resources\\networks\\training-" + _token + "\\" + std::to_string(i) + ".net")));
	}

	this->_data->gameObjectManager.addEntity(&this->_population.at(_currentNetwork), PLAYER_LAYER);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.2f);
}

void ValidationState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_levels.clear();
}

void ValidationState::handleEvents()
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
		}
	}
}

void ValidationState::update(float dt)
{

	NNControlledPlayer& player = _population.at(_currentNetwork);

	player.getNetworkController()->run(player.controllersViewOfLevel());
	std::vector<float> output = player.getNetworkController()->getOutput();
	//given the outputs of the network 

	if (output.size() == 2) {
		//go left 
		if (output.at(0) > 0.9f) {
			player.right();
		}
		else if (output.at(0) < -0.9f) {
			player.left();
		}
		else {
			player.stop();
		}
		// Jump
		if (output.at(1) > 0.9f) {
			player.jump();
		}
		else {
			player.stopJumping();
		}
	}
	this->_data->gameObjectManager.update(dt);

	if (this->_checkProgressClock.getElapsedTime().asSeconds() > 0.5f) {
		this->_checkProgressClock.restart();
		if (player.isAlive() && !player.isFinished()) {
			if (!player.isMakingProgress()) {
				player.die();
			}
		}
	}

	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl) || !player.isAlive()) {
		this->_ttlClock.restart();

		if (player.isAlive() && !player.isFinished()) {
			player.die();
		}

		if (_currentLevel < int(_levels.size())-1) {
			this->_currentLevel++;
			player.nextLevel();
			player.restart();
		}
		else {
			nextNetwork();
			player.selectLevel(_currentLevel);
		}
	}
}

void ValidationState::draw(float dt)
{
	this->_data->window.clear(sf::Color(234, 212, 170, 255));

	this->_data->camera.update(_population.at(_currentNetwork).getSpriteCenterPosition());

	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}

void ValidationState::nextNetwork()
{
	//clear the player layer then add the next player
	this->_data->gameObjectManager.clearEntitiesInLayer(PLAYER_LAYER);
	if (_currentNetwork < int(_population.size())) {
		this->_data->gameObjectManager.addEntity(&_population.at(++_currentNetwork), PLAYER_LAYER);
	}
	else {
		//the cycle has finsihed
		this->_data->stateMachine.popState();
	}
	this->_currentLevel = 0;
}
