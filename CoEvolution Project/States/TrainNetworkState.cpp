#include "TrainNetworkState.h"
#include "TrainingPauseState.h"
#include "ValidationState.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include "../Framework/DEFINITIONS.h"

#define DEFUALT_TRAINING_POPULATION_SIZE 100
#define STARTING_TRAINING_MUTATION_RATE 0.90f
#define TRAINING_MUTATION_RATE 0.90f
#define DEFUALT_TRAINING_TIME_TO_LIVE 1000.0f
#define PASS_PERCENT_NEEDED (1.0f/DEFUALT_TRAINING_POPULATION_SIZE)


#define UP 2
#define DOWN 2
#define LEFT 1
#define RIGHT 2

#define INPUT_LAYER_SIZE (UP+1+DOWN) * (LEFT+1+RIGHT)	

TrainNetworkState::TrainNetworkState(GameDataRef data) : _data(data), _ttl(DEFUALT_TRAINING_TIME_TO_LIVE)
{


}

void TrainNetworkState::init()
{
	this->_levels = std::vector<Level>();
	this->_token = std::to_string(time(0));
	std::string newFolder =  "Resources\\networks\\training-" + _token;
	CreateDirectory(newFolder.c_str(), NULL);


	_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-4", 10.0f));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-1", 10.0f));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_PATH"lvl-2", 10.0f));

	//load the levels in the order to play them;
	//_levels.push_back(Level(_data, TRAINING_LEVEL_1, LEVEL_1_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_2, LEVEL_2_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_3, LEVEL_3_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_4, LEVEL_4_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_5, LEVEL_5_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_6, LEVEL_6_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_7, LEVEL_7_TIME));
	//_levels.push_back(Level(_data, TRAINING_LEVEL_8, LEVEL_8_TIME));

	_info.setFont(this->_data->assetManager.getFont("Menu Font"));
	_info.setCharacterSize(20);
	_info.setFillColor(sf::Color::Black);
	_info.setPosition(sf::Vector2f(0, 0));

	std::vector<std::string> levelNames = std::vector<std::string>();
	for (Level l : _levels) {
		levelNames.push_back(l.getFileName());
	}

	_ga = NeuralNetworkGA(NeuralNetwork::generatePopulation(DEFUALT_TRAINING_POPULATION_SIZE, { INPUT_LAYER_SIZE, 2 }), STARTING_TRAINING_MUTATION_RATE, levelNames);

	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _ga.getPopulation();
	for (int i = 0; i < (int)_ga.getPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), &gapop.at(i), UP, DOWN, LEFT, RIGHT));
	}
	for (NNControlledPlayer& n : _playerPopulation) {
		this->_data->gameObjectManager.addEntity(&n, PLAYER_LAYER);
	}
	_populationChunk = std::vector<NNControlledPlayer*>();
	this->nextPopulationChunk();
	this->selectLevelForChunk();

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(0.9f);
	//the init ttl should be v small just so the networks can rapidly get to the point where they have some features to evolve.
	_info.setString("Controller View Size: " + std::to_string(INPUT_LAYER_SIZE) + "\nPopulation Size:" + std::to_string(this->_playerPopulation.size()) + "\nGeneration [" + std::to_string(this->_ga.getGeneration()) + "] \nAverage Fitness: " + std::to_string(this->_ga.averageFitness()) + "\nBest Fitness: 0");

	this->_data->releaseAccumulator = true;
}

void TrainNetworkState::cleanup()
{
	NeuralNetwork::_networkCount = 0;
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_playerPopulation.clear();
	this->_levels.clear();
	this->_data->releaseAccumulator = false;
}

void TrainNetworkState::resume()
{
	this->_data->releaseAccumulator = true;
	if (this->_data->stateMachine.getSTF() == EXIT) {
		this->_data->releaseAccumulator = false;
		this->_data->stateMachine.popState();
		this->_data->stateMachine.resetSTF();
	}
}

void TrainNetworkState::pause()
{
	this->_data->releaseAccumulator = false;
}

void TrainNetworkState::handleEvents()
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
				this->_ga.saveGAData(this->_token);
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

void TrainNetworkState::update(float dt)
{
	//check all the players have died then eval them
	std::vector<float> output = std::vector<float>();
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		if (nnplayer->isAlive() && !nnplayer->isFinished()) {
			//need to get a set of inputs from the ray cast info from each of the players
			nnplayer->getNetworkController()->run(nnplayer->controllersViewOfLevel());
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
				else {
					nnplayer->stop();
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
	ticks++;

}

void TrainNetworkState::draw(float dt)
{
	NNControlledPlayer* bestController = getBestController();

	if (bestController != nullptr) {
		if (bestController->getNetworkController()->getFitnessScore() > 1.0f) {
			_ttl = DEFUALT_TRAINING_TIME_TO_LIVE;
			this->_ga.setMutationRate(TRAINING_MUTATION_RATE);
		}
		if (this->_displayTraining) {
			bestController->setColor(sf::Color::Red);
			this->_data->camera.update(bestController->getSpriteCenterPosition());
		}
	}
	else {
		if (this->_displayTraining) {
			this->_data->camera.update(this->_levels.at(this->_currentLevel).getCheckpoint(0));
		}
	}

	//check the progess for the controllers and if any have not made progress in 1 second
	checkProgress(0.1f);

	if (this->_ttlClock.getElapsedTime().asSeconds() > (this->_ttl) || areAllDead()) {
		this->_ttlClock.restart();
		//if there are any controllers still alive after the ttl clock 
		//killing the rest off will give them a fitnessScore
		for (NNControlledPlayer* nnplayer : this->_populationChunk) {
			if (nnplayer->isAlive() && !nnplayer->isFinished()) {
				nnplayer->die();
			}
		}

		if (this->_lastChunk) {
			//work out the times of the players then add the corrospoding fitness to that player
			//sort the network based on the player time they got, low to high
			this->_ga.evalutePopulation();

			//find the best controller

			int nnPassed = this->_ga.numberOfNNAboveFitness(100.0f);
			NNControlledPlayer& bestController = this->_playerPopulation.front();

			_info.setString("Controller View Size: " + std::to_string(INPUT_LAYER_SIZE) + "\nPopulation Size:" + std::to_string(this->_playerPopulation.size()) + "\nGeneration [" + std::to_string(this->_ga.getGeneration()) + "] \nAverage Fitness: " + std::to_string(this->_ga.averageFitness()) + "\nBest Fitness: " + std::to_string(bestController.getNetworkController()->getFitnessScore()) + "\nPPC: " + std::to_string(nnPassed) + " of " + std::to_string((int)(PASS_PERCENT_NEEDED*DEFUALT_TRAINING_POPULATION_SIZE)));


			if (((float)nnPassed / (int)_playerPopulation.size()) >= PASS_PERCENT_NEEDED) {
				//we could add another level in when the level has reached satasfactory pass rate
				/*
				int levelsToPass = 10;
				static int levelsCompleted = 0;

				if (++levelsCompleted < levelsToPass) {
					Level nextlvl = Level(this->_levels.front(), Level(Noise::GenHeightMap(sf::Vector2i(20, 4), 3, 2, 1), _data, "levelgentest-1", 15.0f), "recursionlvl");
					this->_levels.pop_back();
					this->_levels.push_back(nextlvl);
				}
				else {
					this->_ga.solved();
				}
				*/
				/*
				bool check = false;
				if (!check) {
					std::cout << "Generating Levels" << std::endl;
					this->_levels.pop_back();
					std::vector<Level> levelgen = std::vector<Level>();
					for (int i = 0; i < 20; i++) {
						_levels.push_back(Level(Noise::GenHeightMap(sf::Vector2i(10, 5), 4, 2, 1), _data, "levelgentest-" + std::to_string(i), 15.0f));
					}
				}
				else {
					this->_ga.solved();
				}
				*/
				this->_ga.solved();
			}

			if (!_ga.isSolved()) {
				//this->_ga.saveFittestNetwork(this->_token);
				this->_ga.nextGeneration();
				//this->_levels.clear();

				std::vector<NeuralNetwork>& gapop = this->_ga.getPopulation();
				for (int i = 0; i < (int)gapop.size(); i++) {
					NNControlledPlayer& nnplayer = this->_playerPopulation.at(i);
					nnplayer.restart();
					nnplayer.setNNController(&gapop.at(i));
				}
			}
			else {
				this->_ga.savePopulation(this->_token);
				this->_ga.saveGAData(this->_token);
				std::cout << "Player has beaten the game, well done!\n" << std::endl;
				//here we can the push the validation set onto the state machine
				this->_data->stateMachine.pushState(StateRef(new ValidationState(this->_data, this->_token, DEFUALT_TRAINING_POPULATION_SIZE* (50.0f/100.0f))), true);
			}
		}
		this->_lastChunk = this->nextPopulationChunk();
		this->selectLevelForChunk();
	}

	this->_data->window.clear(sf::Color(234, 212, 170, 255));

	this->_info.setPosition(this->_data->camera.getCameraBox().left, this->_data->camera.getCameraBox().top);
	this->_data->window.draw(_info);
	if (this->_displayTraining) {
		this->_levels.at(this->_currentLevel).draw();
		this->_data->gameObjectManager.draw(dt);
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
	if (this->_checkProgressClock.getElapsedTime().asSeconds() > (interval)) {
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

bool TrainNetworkState::areAllDead()
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
	int randlevel = NeuralNetwork::randomInt(0, this->_levels.size() - 1);
	for (NNControlledPlayer* nnplayer : this->_populationChunk) {
		nnplayer->selectLevel(randlevel);
		nnplayer->restart();
		this->_currentLevel = randlevel;
	}
}

