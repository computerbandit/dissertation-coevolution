#include "CoEvolveState.h"
#include "TrainingPauseState.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include "../Framework/DEFINITIONS.h"

#define DEFUALT_TRAINING_POPULATION_SIZE 100
#define STARTING_TRAINING_MUTATION_RATE 0.90f
#define TRAINING_MUTATION_RATE 0.90f
#define DEFUALT_TRAINING_TIME_TO_LIVE 1000.0f
#define PASS_PERCENT_NEEDED (1.0f/DEFUALT_TRAINING_POPULATION_SIZE)
#define MAX_GENERATIONS 100

#define UP 3
#define DOWN 3
#define LEFT 1
#define RIGHT 3

#define INPUT_LAYER_SIZE (UP+1+DOWN) * (LEFT+1+RIGHT)	

CoEvolveState::CoEvolveState(GameDataRef data) : _data(data), _ttl(DEFUALT_TRAINING_TIME_TO_LIVE)
{
	this->_maxGenerations = MAX_GENERATIONS;
}

void CoEvolveState::init()
{
	this->_levelPopulation = std::vector<Level>();
	this->_token = std::to_string(time(0));


	std::string newFolder = "Resources/coevo/training-" + _token;
	CreateDirectory(newFolder.c_str(), NULL);
	newFolder = "Resources/coevo/training-" + _token + "/networks";
	CreateDirectory(newFolder.c_str(), NULL);
	newFolder = "Resources/coevo/training-" + _token + "/levels";
	CreateDirectory(newFolder.c_str(), NULL);


	for (int i = 0; i < 5; i++) {
		_levelPopulation.push_back(Level(_data, VALIDATION_LEVEL_1, LEVEL_1_TIME));
		_levelPopulation.push_back(Level(_data, VALIDATION_LEVEL_2, LEVEL_1_TIME));
		_levelPopulation.push_back(Level(_data, VALIDATION_LEVEL_3, LEVEL_1_TIME));
		_levelPopulation.push_back(Level(_data, VALIDATION_LEVEL_4, LEVEL_1_TIME));
		_levelPopulation.push_back(Level(_data, VALIDATION_LEVEL_5, LEVEL_1_TIME));
	}
	/*
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-0", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-1", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-2", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-3", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-4", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-5", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-6", LEVEL_1_TIME));
	_levelPopulation.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-7", LEVEL_1_TIME));
	*/


	_levelGA = GeneticAlgo<Level>(_levelPopulation, 0.90f, std::vector<std::string>());

	_info.setFont(this->_data->assetManager.getFont("Menu Font"));
	_info.setCharacterSize(20);
	_info.setFillColor(sf::Color::Black);
	_info.setPosition(sf::Vector2f(0, 0));

	std::vector<std::string> levelNames = std::vector<std::string>();
	for (Level l : _levelPopulation) {
		levelNames.push_back(l.getFileName());
	}

	_networkGA = GeneticAlgo<NeuralNetwork>(NeuralNetwork::generatePopulation(DEFUALT_TRAINING_POPULATION_SIZE, { INPUT_LAYER_SIZE, 6 , 2 }), STARTING_TRAINING_MUTATION_RATE, std::vector<std::string>());

	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _networkGA.getPopulation();
	for (int i = 0; i < (int)_networkGA.getPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, &_levelPopulation, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), &gapop.at(i), UP, DOWN, LEFT, RIGHT));
	}
	for (NNControlledPlayer& n : _playerPopulation) {
		this->_data->gameObjectManager.addEntity(&n, PLAYER_LAYER);
		this->_tornMatrix.push_back(std::vector<float>());
	}


	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(0.9f);
	//the init ttl should be v small just so the networks can rapidly get to the point where they have some features to evolve.
	//_info.setString("Controller View Size: " + std::to_string(INPUT_LAYER_SIZE) + "\nPopulation Size:" + std::to_string(this->_playerPopulation.size()) + "\nGeneration [" + std::to_string(this->_networkGA.getGeneration()) + "] \nAverage Fitness: " + std::to_string(this->_networkGA.averageFitness()) + "\nBest Fitness: 0");

	this->_data->releaseAccumulator = true;
}

void CoEvolveState::cleanup()
{
	NeuralNetwork::_networkCount = 0;
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_playerPopulation.clear();
	this->_levelPopulation.clear();
	this->_data->releaseAccumulator = false;
}

void CoEvolveState::resume()
{
	this->_data->releaseAccumulator = true;
	if (this->_data->stateMachine.getSTF() == EXIT) {
		this->_data->releaseAccumulator = false;
		this->_data->stateMachine.popState();
		this->_data->stateMachine.resetSTF();
	}
}

void CoEvolveState::pause()
{
	this->_data->releaseAccumulator = false;
}

void CoEvolveState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			this->_data->window.close();
			break;
		case sf::Event::Resized:
			this->_data->camera.resize(event);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::S:
				this->_networkGA.writeGAData("coevo/training-", this->_token, "networks");
				this->_levelGA.writeGAData("coevo/training-", this->_token, "levels");
				std::cout << "GA DATA Saved!" << std::endl;
				break;
			case sf::Keyboard::D:
				if (this->_displayTraining)
				{
					this->_displayTraining = false;
				}
				else {
					this->_displayTraining = true;
				}

				std::cout << "Toggle Display!" << std::endl;
				break;
			case sf::Keyboard::A:
				if (this->_data->releaseAccumulator)
				{
					this->_data->releaseAccumulator = false;
				}
				else {
					this->_data->releaseAccumulator = true;
				}

				std::cout << "Toggle Accumulator Lock" << std::endl;
				break;
			case sf::Keyboard::Escape:
				this->_data->stateMachine.pushState(StateRef(new TrainingPauseState(_data)), false);
				break;
			}
			break;
		}
	}
}

void CoEvolveState::update(float dt)
{
	//check all the players have died then eval them
	std::vector<float> output = std::vector<float>();
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
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

	//updating all nncontrolled player objects in the game
	this->_data->gameObjectManager.update(dt);
	//find the nnplayer that has made the most progess if we are displaying then set th cameras postition to the best controller
	ticks++;

}

void CoEvolveState::draw(float dt)
{
	NNControlledPlayer* bestController = getBestController();

	if (bestController != nullptr) {
		if (bestController->getNetworkController()->getFitness() > 1.0f) {
			_ttl = DEFUALT_TRAINING_TIME_TO_LIVE;
			this->_networkGA.setMutationRate(TRAINING_MUTATION_RATE);
		}
		if (this->_displayTraining) {
			bestController->setColor(sf::Color::Red);
			this->_data->camera.update(bestController->getSpriteCenterPosition(), sf::Vector2f(1.5f,1.5f));
		}
	}
	else {
		if (this->_displayTraining) {
			this->_data->camera.update(this->_levelPopulation.at(this->_currentLevel).getCheckpoint(0), sf::Vector2f(10,10));
		}
	}

	//check the progess for the controllers and if any have not made progress in 1 second
	checkProgress(0.1f);

	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl) || areAllDead()) {
		this->_ttlClock.restart();
		//if there are any controllers still alive after the ttl clock 
		//killing the rest off will give them a fitnessScore
		int currentNetwork = 0;
		for (NNControlledPlayer& nnplayer : _playerPopulation) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				nnplayer.die();
			}
			//push the fitness value to the torn matrix two dimentional array;
			float cappedFitness = nnplayer.getNetworkController()->getFitness();
			cappedFitness = (cappedFitness >= 100.0f) ? 100.0f : cappedFitness;
			this->_tornMatrix.at(currentNetwork++).push_back(cappedFitness);
		}

		if (++_currentLevel < int(_levelPopulation.size())) {
			for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
				nnplayer.selectLevel(_currentLevel);
				nnplayer.restart();
			}
		}
		else {
			this->_currentLevel = 0;

			//this means that one cycle has happend so eval the populations and then pass the new generation
			this->saveCoEvoData("coevo/training-", _token, "");

			std::vector<int> playersPassedLevel = std::vector<int>(int(this->_levelPopulation.size()));
			for (int& i : playersPassedLevel) {
				i = 0;
			}

			//evaluate the percentage of the player population success on each of the levels
			for (int i = 0; i < int(this->_playerPopulation.size()); i++) {
				float sum = 0.0f;
				float averageFitness = 0.0f;
				for (int j = 0; j < int(this->_tornMatrix.at(i).size()); j++){
					sum += this->_tornMatrix.at(i).at(j);
					if (sum == 0.0f) {
						averageFitness = 0.0f;
					}
					else {
						averageFitness = float(sum / this->_levelPopulation.size());
					}

					if (this->_tornMatrix.at(i).at(j)) {
						playersPassedLevel.at(j)++;
					}
					
					this->_playerPopulation.at(i).getNetworkController()->setFitness(averageFitness);
				}
			}

			std::vector<Level>& galevels = this->_levelGA.getPopulation();
			
			for (int i = 0; i < int(this->_levelPopulation.size()); i++)
			{
				float fitness = float(playersPassedLevel.at(i)) / int(this->_playerPopulation.size());
				if (fitness == 0.0f) {
					fitness = 1.0f;
				}else if (fitness <= 0.10f) {
					fitness += 0.50f;
				}
				galevels.at(i).setFitness(100.0f - (fitness * 100.0f));
			}
			

			this->_levelGA.evalutePopulation();
			this->_levelGA.nextGeneration();
			this->_levelPopulation = this->_levelGA.getPopulation();
			this->_networkGA.evalutePopulation();
			this->_networkGA.nextGeneration();

			std::string newFolder = "Resources/coevo/training-" + _token;
			newFolder = "Resources/coevo/training-" + _token + "/networks/gen-"+std::to_string(this->_networkGA.getGeneration());
			CreateDirectory(newFolder.c_str(), NULL);
			newFolder = "Resources/coevo/training-" + _token + "/levels/gen-" + std::to_string(this->_levelGA.getGeneration());
			CreateDirectory(newFolder.c_str(), NULL);


			this->_networkGA.savePopulation("coevo/training-", this->_token, "networks/gen-" + std::to_string(this->_networkGA.getGeneration()));
			this->_networkGA.writeGAData("coevo/training-", this->_token, "networks");
			this->_levelGA.savePopulation("coevo/training-", this->_token, "levels/gen-" + std::to_string(this->_levelGA.getGeneration()));
			this->_levelGA.writeGAData("coevo/training-", this->_token, "levels");


			this->_tornMatrix = std::vector<std::vector<float>>(int(this->_playerPopulation.size()));

			std::vector<NeuralNetwork>& gapop = this->_networkGA.getPopulation();
			for (int i = 0; i < (int)gapop.size(); i++) {
				NNControlledPlayer& nnplayer = this->_playerPopulation.at(i);
				nnplayer.setNNController(&gapop.at(i));
				nnplayer.selectLevel(0);
				nnplayer.restart();
				//nnplayer.setLevelPtr(&_levelPopulation);
				_tornMatrix.push_back(std::vector<float>());

			}

			if (_levelGA.getGeneration() >= _maxGenerations) {
				_levelGA.solved();
				_networkGA.solved();
			}

		}


		if (_networkGA.isSolved() || _levelGA.isSolved()) {
			this->_networkGA.savePopulation("coevo/training-", this->_token, "networks");
			this->_networkGA.writeGAData("coevo/training-", this->_token, "networks");
			this->_levelGA.savePopulation("coevo/training-", this->_token, "levels");
			this->_levelGA.writeGAData("coevo/training-", this->_token, "levels");
			std::cout << "Training has reach max generations!" << std::endl;
			//here we can the push the validation set onto the state machine
			this->_data->stateMachine.popState();
		}
	}

	

	this->_data->window.clear(sf::Color(234, 212, 170, 255));

	this->_info.setPosition(this->_data->camera.getCameraBox().left, this->_data->camera.getCameraBox().top);
	this->_data->window.draw(_info);
	if (this->_displayTraining) {
		this->_levelPopulation.at(this->_currentLevel).draw();
		this->_data->gameObjectManager.draw(dt);
	}
	this->_data->window.display();
}

NNControlledPlayer * CoEvolveState::getBestController()
{
	float mostProgress = 0.0f;
	NNControlledPlayer* bestController = nullptr;
	for (NNControlledPlayer& nnplayer : _playerPopulation) {
		nnplayer.setColor(sf::Color::White);
		if (nnplayer.getProgress() > mostProgress) {
			bestController = &(nnplayer);
			mostProgress = nnplayer.getProgress();
		}
	}
	return bestController;
}

void CoEvolveState::checkProgress(float interval)
{
	//checks if the controller has made progress in the last second if not it dies
	if (this->_checkProgressClock.getElapsedTime().asSeconds() > (interval)) {
		this->_checkProgressClock.restart();
		for (NNControlledPlayer& nnplayer : _playerPopulation) {
			if (nnplayer.isAlive() && !nnplayer.isFinished()) {
				if (!nnplayer.isMakingProgress()) {
					nnplayer.die();
				}
			}
		}
	}
}

bool CoEvolveState::areAllDead()
{
	bool allDead = false;
	for (NNControlledPlayer& nnplayer : _playerPopulation) {
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

void CoEvolveState::saveCoEvoData(std::string path, std::string token, std::string subfolder)
{

	sf::Image matrix;
	matrix.create(int(this->_levelPopulation.size()), int(this->_playerPopulation.size()), sf::Color(0, 0, 0));
	float delta = 255.0f / 100.0f;
	for (int i = 0; i < int(this->_tornMatrix.size()); i++) {
		std::vector<float>& playerResults = this->_tornMatrix.at(i);
		for (int j = 0; j < int(playerResults.size()); j++) {
			float mono = playerResults.at(j) * delta;
			matrix.setPixel(j, i, sf::Color(mono, mono, mono, 255));
		}
	}
	sf::Texture text;
	text.create(int(this->_levelPopulation.size()), int(this->_playerPopulation.size()));
	text.update(matrix.getPixelsPtr());

	text.copyToImage().saveToFile("Resources/" + path + token + "/" + subfolder + "/matrix-" + std::to_string(this->_levelGA.getGeneration()) + ".png");
}
