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
	this->_tornMatrix = std::vector<std::vector<float>>();
	this->_tornMatrix.push_back(std::vector<float>());
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
	/*
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-0", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-1", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-2", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-3", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-4", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-6", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-7", 10.0f));
	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-8", 10.0f));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-9", 10.0f));
	*/

	std::ofstream file;
	//load in the population
	for (int i = 0; i < _populationSize; i++) {
		this->_population.push_back(NNControlledPlayer(this->_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), new NeuralNetwork("Resources\\networks\\training-" + _token + "\\" + std::to_string(i) + ".net")));
	}

	this->_data->gameObjectManager.addEntity(&this->_population.at(_currentNetwork), PLAYER_LAYER);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.2f);

	this->_data->releaseAccumulator = true;
}

void ValidationState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_levels.clear();
	this->_population.clear();
	this->_data->releaseAccumulator = false;
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
				this->_data->releaseAccumulator = false;
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

	if (this->_checkProgressClock.getElapsedTime().asSeconds() > 0.2f) {
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

		//push the fitness value to the torn matrix two dimentional array;
		float cappedFitness = player.getNetworkController()->getFitnessScore();
		cappedFitness = (cappedFitness >= 100.0f) ? 100.0f : cappedFitness;
		this->_tornMatrix.at(this->_currentNetwork).push_back(cappedFitness);


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
	if (_currentNetwork < int(_population.size())-1) {
		this->_tornMatrix.push_back(std::vector<float>());
		this->_data->gameObjectManager.addEntity(&_population.at(++_currentNetwork), PLAYER_LAYER);
	}
	else {
		//the cycle has finsihed
		for (int i = 0; i < int(this->_tornMatrix.size()); i++) {
			std::vector<float>& playerResults = this->_tornMatrix.at(i);
			for (int j = 0; j < int(playerResults.size()); j++) {
				this->_valiData.append(std::to_string(playerResults.at(j)));
				if (j < int(playerResults.size()) - 1) {
					this->_valiData.append(",");
				}

			}
			if (i < int(this->_tornMatrix.size()) - 1) {
				this->_valiData.append("\n");
			}
		}

		this->saveValiData(this->_token);
		this->_data->stateMachine.popState();
	}
	this->_currentLevel = 0;
}

void ValidationState::saveValiData(std::string token)
{

	sf::Image matrix;
	matrix.create(int(this->_levels.size()), int(this->_population.size()), sf::Color(0,0,0));
	float delta = 255.0f/100.0f;
	for (int i = 0; i < int(this->_tornMatrix.size()); i++) {
		std::vector<float>& playerResults = this->_tornMatrix.at(i);
		for (int j = 0; j < int(playerResults.size()); j++) {
			float mono = playerResults.at(j) * delta;
			matrix.setPixel(j, i, sf::Color(mono, mono, mono, 255));
		}
	}
	sf::Texture text;
	text.create(int(this->_levels.size()), int(this->_population.size()));
	text.update(matrix.getPixelsPtr());

	text.copyToImage().saveToFile("Resources/networks/training-" + token + "/matrix.png");


	std::ofstream csv;
	if (csv.is_open()) {
		csv.close();
	}
	csv.open("Resources/networks/training-" + token + "/validationdata.csv");
	csv << this->_valiData;
	csv.close();
}