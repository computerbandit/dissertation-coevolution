#include "TrainNetworkState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include "DEFINITIONS.h"
#include "TestNetworkState.h"

TrainNetworkState::TrainNetworkState(GameDataRef data, float timetolive, float speedMultiplier, bool display): _data(data), _display(display), _ttl(timetolive)
{
	_ga = NeuralNetworkGA(NeuralNetwork::generatePopulation(DEFUALT_TRAINNING_POPULATION_SIZE, {INPUT_LAYER_SIZE, 4, 3}), STARTING_TRAINNING_MUTATION_RATE);
	this->_levels = std::vector<Level>();
	this->_data->gameSpeedMultiplier = speedMultiplier;
	this->_token = std::to_string(time(0));
}

void TrainNetworkState::init()
{
	//load the texturesheet
	this->_data->assetManager.loadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));

	//load the levels in the order to play them;
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));

	this->_data->assetManager.loadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(16, 16));

	_info.setFont(this->_data->assetManager.getFont("Menu Font"));
	_info.setCharacterSize(20);
	_info.setFillColor(sf::Color::Black);
	_info.setPosition(sf::Vector2f(0, 0));

	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _ga.getPopulation();
	for (int i = 0; i < (int)_ga.getPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, _levels, _currentLevel, sf::Vector2f(16, 16), &gapop.at(i)));
	}
	for (NNControlledPlayer& n : _playerPopulation) {
		this->_data->gameObjectManager.addEntity(&n);
	}

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	//the init ttl should be v small just so the networks can rapidly get to the point where they have some features to evolve.
}

void TrainNetworkState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_playerPopulation.clear();
	this->_levels.clear();
}

void TrainNetworkState::handleEvents()
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
				this->_data->gameSpeedMultiplier = 5.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				this->_data->gameSpeedMultiplier = 10.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				this->_data->gameSpeedMultiplier = 100.0f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				this->_data->stateMachine.popState();
			}
		}

		
	}
}

void TrainNetworkState::update(float dt)
{
	//check all the players have died then eval them
	std::vector<float> output = std::vector<float>();
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.isAlive() && !nnplayer.isFinished()) {
			//need to get a set of inputs from the ray cast info from each of the players
			nnplayer.getNetworkController()->run(nnplayer.controllersViewOfLevel(CONTROLLER_TILES_VIEW));
			 output = nnplayer.getNetworkController()->getOutput();
			//given the outputs of the network 
			
			 if (output.size() == 3) {
				 //go left 
				 if (output.at(0) > 0.9f) {
					 nnplayer.right();
				 }
				 // go right
				 if (output.at(1) > 0.9f) {
					 nnplayer.left();
				 }
				 // Jump
				 if (output.at(2) > 0.9f) {
					 nnplayer.jump();
				 }
				 else {
					 nnplayer.stopJumping();
				 }
			 }
		}
	}

	//updating all nncontrolled player objects in the game
	this->_data->gameObjectManager.update(dt);
	//find the nnplayer that has made the most progess if we are displaying then set th cameras postition to the best controller
	
}

void TrainNetworkState::draw(float dt)
{

	float mostProgress = 0.0f;
	NNControlledPlayer* bestController = nullptr;
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.getProgress() > mostProgress) {
			bestController = &nnplayer;
			mostProgress = nnplayer.getProgress();
		}
	}

	if (bestController != nullptr) {
		if (_ttl < 1.0f && mostProgress > 0.0f) {
			_ttl = DEFUALT_TRAINNGNG_TIME_TO_LIVE;
		}
		if (_display) {
			bestController->setColor(sf::Color::Red);
			this->_data->camera.update(bestController->getPosition());
		}
	}
	else {
		if (_display) {
			this->_data->camera.update(this->_levels.at(this->_currentLevel).getCheckpoint(0));
		}
	}

	//check the progess for the controllers and if any have not made progress in 1 second
	if (this->_checkProgressClock.getElapsedTime().asSeconds() > (1.0f / this->_data->gameSpeedMultiplier)) {
		this->_checkProgressClock.restart();
		for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				if (!nnplayer.isMakingProgress()) {
					nnplayer.die();
				}
			}
		}
	}
	bool allDead = false;
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.isAlive()) {
			allDead = false;
			break;
		}
		else {
			allDead = true;
		}
	}
	
	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl / this->_data->gameSpeedMultiplier) || allDead) {
		this->_ttlClock.restart();
		//if there are any controllers still alive after the ttl clock 
		//killing the rest off will give them a fitnessScore
		for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				nnplayer.die();
			}
		}
		this->_ga.evalutePopulation();

		//find the best controller
		NNControlledPlayer bestController = this->_playerPopulation.front();
		float mostProgess = bestController.getProgress();
		float averageProgress = this->_ga.averageFitness();

		std::cout << "Generation [" << this->_ga.getGeneration() << "] -> Percentage Progress: " << averageProgress << "% average, " << mostProgess << "% best controller" << "\r";


		if (bestController.getNetworkController()->getFitnessScore() >= 100.0f) {
			if (_currentLevel + 1 < (int)this->_levels.size()) {
				std::cout << "\n Level Completed" << std::endl;
				this->_currentLevel++;
				this->_ga.setMutationRate(0.2f);
				this->_ga.saveFittestNetwork(this->_token);
				//set the rest of the population controllers to the best one
				for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
					nnplayer.restart();
				}
			}
			else {
				this->_ga.solved();
			}
		}


		if (!_ga.isSolved()) {
			this->_ga.saveFittestNetwork(this->_token);
			this->_ga.nextGeneration();
			std::vector<NeuralNetwork>& gapop = this->_ga.getPopulation();
			for (int i = 0; i < (int)gapop.size(); i++) {
				NNControlledPlayer& nnplayer = this->_playerPopulation.at(i);
				nnplayer.restart();
				nnplayer.setNNController(&gapop.at(i));
			}
		}
		else {
			this->_ga.saveFittestNetwork(this->_token);
			std::cout << "Player has beaten the game, well done!\n" << std::endl;
			this->_data->stateMachine.popState();
		}

	}
	this->_data->window.clear(sf::Color::White);
	if (this->_display) {
		_info.setString("Speed: " + std::to_string(this->_data->gameSpeedMultiplier) + "x , TTL: " + std::to_string(_ttlClock.getElapsedTime().asSeconds()) + " / " + std::to_string(_ttl/this->_data->gameSpeedMultiplier));
		_info.setPosition(this->_data->camera.getCameraBox().left, this->_data->camera.getCameraBox().top);

		this->_levels.at(this->_currentLevel).draw();
		this->_data->gameObjectManager.draw(dt);

		this->_data->window.draw(_info);
	}
	this->_data->window.display();
}

