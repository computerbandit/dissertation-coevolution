#include "TrainNetworkState.h"
#include "TrainingPauseState.h"
#include <string>
#include <iostream>
#include "../Framework/DEFINITIONS.h"

#define DEFUALT_TRAINING_POPULATION_SIZE 100
#define STARTING_TRAINING_MUTATION_RATE 0.95f
#define TRAINING_MUTATION_RATE 0.99f
#define DEFUALT_TRAINING_TIME_TO_LIVE 30.0f
#define DEFUALT_TRAINING_SPEED_MULTIPLIER 1.0f
#define PASS_PERCENT_NEEDED 0.99f

#define INPUT_LAYER_SIZE (2+1+2) * (2+1+2)	

TrainNetworkState::TrainNetworkState(GameDataRef data, bool display): _data(data), _display(display), _ttl(DEFUALT_TRAINING_TIME_TO_LIVE)
{
	_ga = NeuralNetworkGA(NeuralNetwork::generatePopulation(DEFUALT_TRAINING_POPULATION_SIZE, {INPUT_LAYER_SIZE, 6, 6 , 2}), STARTING_TRAINING_MUTATION_RATE);
	this->_levels = std::vector<Level>();
	this->_data->gameSpeedMultiplier = DEFUALT_TRAINING_SPEED_MULTIPLIER;
	this->_token = std::to_string(time(0));
}

void TrainNetworkState::init()
{
	//load the levels in the order to play them;
	_levels.push_back(Level(_data, TRAINING_LEVEL_1, LEVEL_1_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_2, LEVEL_2_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_3, LEVEL_3_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_4, LEVEL_4_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_5, LEVEL_5_TIME));
	_levels.push_back(Level(_data, TRAINING_LEVEL_6, LEVEL_6_TIME));
	_levels.push_back(Level(_data, TRAINING_LEVEL_7, LEVEL_7_TIME));
	//_levels.push_back(Level(_data, TRAINNING_LEVEL_8, LEVEL_8_TIME));

	_info.setFont(this->_data->assetManager.getFont("Menu Font"));
	_info.setCharacterSize(20);
	_info.setFillColor(sf::Color::Black);
	_info.setPosition(sf::Vector2f(0, 0));

	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _ga.getPopulation();
	for (int i = 0; i < (int)_ga.getPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, &_levels, sf::Vector2f(TILE_SIZE/4, TILE_SIZE/4), &gapop.at(i)));
	}
	for (NNControlledPlayer& n : _playerPopulation) {
		this->_data->gameObjectManager.addEntity(&n);
	}
	_populationChunk = std::vector<NNControlledPlayer*>();
	this->nextPopulationChunk();
	this->selectLevelForChunk();

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.2f);
	//the init ttl should be v small just so the networks can rapidly get to the point where they have some features to evolve.
	_info.setString("Controller View Size: " + std::to_string(INPUT_LAYER_SIZE) + "\nPopulation Size:" + std::to_string(this->_playerPopulation.size()) + "\nGeneration [" + std::to_string(this->_ga.getGeneration()) + "] \nAverage Fitness: " + std::to_string(this->_ga.averageFitness()) + "\nBest Fitness: 0\nSpeed: " + std::to_string(this->_data->gameSpeedMultiplier) + "x");
}

void TrainNetworkState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_playerPopulation.clear();
	this->_levels.clear();
}

void TrainNetworkState::resume()
{
	if (this->_data->stateMachine.getSTF() == EXIT) {
		this->_data->stateMachine.popState();
		this->_data->stateMachine.resetSTF();
	}
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
			this->_data->camera.setCenter(sf::Vector2f(0, 0));
		}
		
		if (sf::Event::KeyPressed == event.type) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				this->_ga.saveGAData(this->_token);
				std::cout << "GA DATA Saved!" << std::endl;
			}

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
				this->_data->gameSpeedMultiplier = 50.0f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				this->_data->stateMachine.pushState(StateRef(new TrainingPauseState(_data)), false);
			}
		}
	}
}

void TrainNetworkState::update(float dt)
{
	//check all the players have died then eval them
	std::vector<float> output = std::vector<float>();
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		if (nnplayer->isAlive() && !nnplayer->isFinished()) {
			//need to get a set of inputs from the ray cast info from each of the players
			nnplayer->getNetworkController()->run(nnplayer->controllersViewOfLevel(2, 2, 2, 2));
			 output = nnplayer->getNetworkController()->getOutput();
			//given the outputs of the network 
			
			 if (output.size() == 2) {
				 //go left 
				 if (output.at(0) > 0.9f) {
					 nnplayer->right();
				 }
				 else if (output.at(0) < -0.9f) {
					 nnplayer->left();
				 } 
				 // Jump
				 if (output.at(1) > 0.9f) {
					 nnplayer->jump();
				 }
				 else {
					 nnplayer->stopJumping();
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

	NNControlledPlayer* bestController = getBestController();
	

	if (bestController != nullptr) {
		if (bestController->getNetworkController()->getFitnessScore() > 1.0f) {
			_ttl = DEFUALT_TRAINING_TIME_TO_LIVE;
			this->_ga.setMutationRate(TRAINING_MUTATION_RATE);
		}
		if (_display) {
			bestController->setColor(sf::Color::Red);
			this->_data->camera.update(bestController->getSpriteCenterPosition());
		}
	}
	else {
		if (_display) {
			this->_data->camera.update(this->_levels.at(this->_currentLevel).getCheckpoint(0));
		}
	}

	//check the progess for the controllers and if any have not made progress in 1 second
	checkProgress(1.0f);


	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl / this->_data->gameSpeedMultiplier) || AreAllDead()) {
		this->_ttlClock.restart();
		//if there are any controllers still alive after the ttl clock 
		//killing the rest off will give them a fitnessScore
		for (NNControlledPlayer* nnplayer : this->_populationChunk) {
			if (nnplayer->isAlive() && !nnplayer->isFinished()) {
				nnplayer->die();
			}
		}

		if (this->_lastChunk) {
			this->_ga.evalutePopulation();

			//find the best controller

			int nnPassed = this->_ga.numberOfNNAboveFitness(100.0f);
			NNControlledPlayer& bestController = this->_playerPopulation.front();

			_info.setString("Controller View Size: " + std::to_string(INPUT_LAYER_SIZE) + "\nPopulation Size:" + std::to_string(this->_playerPopulation.size()) + "\nGeneration [" + std::to_string(this->_ga.getGeneration()) + "] \nAverage Fitness: " + std::to_string(this->_ga.averageFitness()) + "\nBest Fitness: " + std::to_string(bestController.getNetworkController()->getFitnessScore()) + "\nPPC: " + std::to_string(nnPassed) +" of "+ std::to_string((int)(PASS_PERCENT_NEEDED*DEFUALT_TRAINING_POPULATION_SIZE)) + "\nSpeed: " + std::to_string(this->_data->gameSpeedMultiplier) + "x");


			if (((float)nnPassed/(int)_playerPopulation.size()) >= PASS_PERCENT_NEEDED) {
				this->_ga.solved();
			}
			

			if (!_ga.isSolved()) {
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
				this->_ga.saveGAData(this->_token);
				std::cout << "Player has beaten the game, well done!\n" << std::endl;
				this->_data->stateMachine.popState();
			}
		}	
		this->_lastChunk = this->nextPopulationChunk();
		this->selectLevelForChunk();
	}

	this->_data->window.clear(sf::Color(234, 212, 170, 255));
	if (this->_display) {
		_info.setPosition(this->_data->camera.getCameraBox().left, this->_data->camera.getCameraBox().top);

		this->_levels.at(this->_currentLevel).draw();
		this->_data->gameObjectManager.draw(dt);
		this->_data->window.draw(_info);
	}
	this->_data->window.display();
}

bool TrainNetworkState::nextPopulationChunk()
{
	for (NNControlledPlayer& n : this->_playerPopulation) {
		n.deactivate();
	}
	this->_populationChunk.clear();
	for (int i = 0; i < this->_chunkSize; i++) {
		if ((i + this->_chunkIndex) >= (int)this->_playerPopulation.size()) {
			break;
		}
		else {
			_populationChunk.push_back(&this->_playerPopulation.at(i + this->_chunkIndex));
			_populationChunk.back()->activate();
		}
	}
	this->_chunkIndex += this->_chunkSize;
	if (this->_chunkIndex >= (int)this->_playerPopulation.size()) {
		this->_chunkIndex = 0;
		return true;
	}
	else {
		return false;
	}
}

NNControlledPlayer * TrainNetworkState::getBestController()
{
	float mostProgress = 0.0f;
	NNControlledPlayer* bestController = nullptr;
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		nnplayer->setColor(sf::Color::White);
		if (nnplayer->getProgress() > mostProgress) {
			bestController = &(*nnplayer);
			mostProgress = nnplayer->getProgress();
		}
	}
	return bestController;
}

void TrainNetworkState::checkProgress(float interval)
{
	//checks if the controller has made progress in the last second if not it dies
	if (this->_checkProgressClock.getElapsedTime().asSeconds() > (interval / this->_data->gameSpeedMultiplier)) {
		this->_checkProgressClock.restart();
		for (NNControlledPlayer* nnplayer : this->_populationChunk) {
			if (nnplayer->isAlive() && !nnplayer->isFinished()) {
				if (!nnplayer->isMakingProgress()) {
					nnplayer->die();
				}
			}
		}
	}
}

bool TrainNetworkState::AreAllDead()
{
	bool allDead = false;
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		if (nnplayer->isAlive()) {
			allDead = false;
			break;
		}
		else {
			allDead = true;
		}
	}
	return allDead;
}

void TrainNetworkState::selectLevelForChunk()
{
	int randlevel = NeuralNetwork::randomInt(0,this->_levels.size()-1);
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		nnplayer->selectLevel(randlevel);
		this->_currentLevel = randlevel;
	}
}

