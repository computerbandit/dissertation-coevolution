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
	for (NNControlledPlayer& n : this->_population) {
		this->_data->gameObjectManager.addEntity(&n, PLAYER_LAYER);
		this->_tornMatrix.push_back(std::vector<float>());
	}

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(400, 100));
	this->_data->camera.zoom(1.8f);

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
	//check all the players have died then eval them
	std::vector<float> output = std::vector<float>();
	for (NNControlledPlayer& nnplayer : this->_population) {
		if (nnplayer.isAlive() && !nnplayer.isFinished()) {
			//need to get a set of inputs from the ray cast info from each of the players
			nnplayer.getNetworkController()->run(nnplayer.controllersViewOfLevel());
			output = nnplayer.getNetworkController()->getOutput();
			//given the outputs of the network 

			if (output.size() == 2) {
				//go left 
				if (output.at(0) > 0.9f) {
					nnplayer.right();
				}
				else if (output.at(0) < -0.9f) {
					nnplayer.left();
				}
				else {
					nnplayer.stop();
				}
				// Jump
				if (output.at(1) > 0.9f) {
					nnplayer.jump();
				}
				else {
					nnplayer.stopJumping();
				}
			}
		}
	}
	
	this->_data->gameObjectManager.update(dt);

	if (this->_checkProgressClock.getElapsedTime().asSeconds() > 0.2f) {
		this->_checkProgressClock.restart();
		for (NNControlledPlayer& nnplayer : this->_population) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				if (!nnplayer.isMakingProgress()) {
					nnplayer.die();
				}
			}
		}
	}

	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl) || areAllDead()) {
		this->_ttlClock.restart();
		int currentNetwork = 0;
		for (NNControlledPlayer& nnplayer : this->_population) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				nnplayer.die();
			}

			//push the fitness value to the torn matrix two dimentional array;
			float cappedFitness = nnplayer.getNetworkController()->getFitness();
			cappedFitness = (cappedFitness >= 100.0f) ? 100.0f : cappedFitness;
			this->_tornMatrix.at(currentNetwork++).push_back(cappedFitness);
		}


		if (_currentLevel < int(_levels.size())-1) {
			this->_currentLevel++;
			for (NNControlledPlayer& nnplayer : this->_population) {
				nnplayer.nextLevel();
				nnplayer.restart();
			}
		}
		else {
			saveValiData("networks/training-",  _token);
			this->_data->stateMachine.popState();
		}
	}
}

void ValidationState::draw(float dt)
{
	this->_data->window.clear(sf::Color(234, 212, 170, 255));

	//this->_data->camera.update(_population.at(_currentNetwork).getSpriteCenterPosition());

	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}

void ValidationState::saveValiData(std::string path, std::string token)
{

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

	text.copyToImage().saveToFile("Resources/" + path  + token + "/matrix.png");


	std::ofstream csv;
	if (csv.is_open()) {
		csv.close();
	}
	csv.open("Resources/" + path + token + "/validationdata.csv");
	csv << this->_valiData;
	csv.close();
}

bool ValidationState::areAllDead()
{
	bool allDead = false;
	for (NNControlledPlayer& nnplayer : this->_population) {
		if (nnplayer.isAlive()) {
			allDead = false;
			break;
		}
		else {
			allDead = true;
		}
	}
	return allDead;
}
