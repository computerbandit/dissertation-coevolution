#include "TrainNetworkState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include "DEFINITIONS.h"
#include "TestNetworkState.h"

TrainNetworkState::TrainNetworkState(GameDataRef data, float timetolive, float speedMultiplier, bool display): _data(data), _display(display), _ttl(timetolive)
{
	_ga = NeuralNetworkGA(NeuralNetwork::GeneratePopulation(DEFUALT_TRAINNING_POPULATION_SIZE, {INPUT_LAYER_SIZE,4, 5 ,  3}), STARTING_TRAINNING_MUTATION_RATE);
	this->_levels = std::vector<Level>();
	this->_data->gameSpeedMultiplier = speedMultiplier;
	this->_token = std::to_string(time(0));
}

void TrainNetworkState::Init()
{

	//load the texturesheet
	this->_data->assetManager.LoadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));

	//load the levels in the order to play them;
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));

	this->_data->assetManager.LoadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(16, 16));

	_info.setFont(this->_data->assetManager.GetFont("Menu Font"));
	_info.setCharacterSize(20);
	_info.setFillColor(sf::Color::Black);
	_info.setPosition(sf::Vector2f(0, 0));

	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _ga.GetPopulation();
	for (int i = 0; i < (int)_ga.GetPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, _levels, _currentLevel, sf::Vector2f(16, 16), &gapop.at(i)));
	}
	for (NNControlledPlayer& n : _playerPopulation) {
		this->_data->gameObjectManager.AddEntity(&n);
	}

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	//the init ttl should be v small just so the networks can rapidly get to the point where they have some features to evolve.
}

void TrainNetworkState::Cleanup()
{
	this->_data->camera.Restore();
	this->_data->gameObjectManager.ClearEntities();
}

void TrainNetworkState::HandleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->Cleanup();
			this->_data->window.close();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);
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
				this->_data->stateMachine.PopState();
			}
		}

		
	}
}

void TrainNetworkState::Update(float dt)
{
	//check all the players have died then eval them
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.IsAlive() && !nnplayer.Finished()) {

			//update the player based on the nn that controls it
			NeuralNetwork* controller = nnplayer.GetNetworkController();
			//need to get a set of inputs from the ray cast info from each of the players

			controller->Run(nnplayer.ConrollersViewOfLevel(CONTROLLER_TILES_VIEW));
			std::vector<float> output = controller->GetOutput();
			//given the outputs of the network 
			
			// go left
			if (output.at(0) > 0.9f) {
				nnplayer.Right();
			}
			// go right
			if (output.at(1) > 0.9f) {
				nnplayer.Left();
			}
			// Jump
			if (output.at(2) > 0.9f) {
				nnplayer.Jump();
			}
			else {
				nnplayer.StopJumping();
			}
		}
	}

	//updating all nncontrolled player objects in the game
	this->_data->gameObjectManager.Update(dt);
	//find the nnplayer that has made the most progess if we are displaying then set th cameras postition to the best controller
	
}

void TrainNetworkState::Draw(float dt)
{

	float mostProgress = 0.0f;
	NNControlledPlayer* bestController = nullptr;
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.GetProgress() > mostProgress) {
			bestController = &nnplayer;
			mostProgress = nnplayer.GetProgress();
		}
	}

	if (bestController != nullptr) {
		if (_ttl < 1.0f && mostProgress > 0.0f) {
			_ttl = DEFUALT_TRAINNGNG_TIME_TO_LIVE;
		}
		if (_display) {
			bestController->SetColor(sf::Color::Red);
			this->_data->camera.Update(bestController->GetPosition());
		}
	}
	else {
		if (_display) {
			this->_data->camera.Update(this->_levels.at(this->_currentLevel).GetCheckpoint(0));
		}
	}

	//check the progess for the controllers and if any have not made progress in 1 second
	if (this->_checkProgressClock.getElapsedTime().asSeconds() > (1.0f / this->_data->gameSpeedMultiplier)) {
		this->_checkProgressClock.restart();
		for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
			if (nnplayer.IsAlive() && !nnplayer.Finished()) {
				if (!nnplayer.IsMakingProgress()) {
					nnplayer.Die();
				}
			}
		}
	}
	bool allDead = false;
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (nnplayer.IsAlive()) {
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
			if (nnplayer.IsAlive() && !nnplayer.Finished()) {
				nnplayer.Die();
			}
		}
		this->_ga.EvalutePopulation();

		//find the best controller
		NNControlledPlayer bestController = this->_playerPopulation.front();
		float mostProgess = bestController.GetProgress();
		float averageProgress = this->_ga.AverageFitness();

		std::cout << "Generation [" << this->_ga.GetGeneration() << "] -> Percentage Progress: " << averageProgress << "% average, " << mostProgess << "% best controller" << "\r";


		if (bestController.GetNetworkController()->GetFitnessScore() >= 100.0f) {
			if (_currentLevel + 1 < (int)this->_levels.size()) {
				std::cout << "\n Level Completed" << std::endl;
				this->_currentLevel++;
				this->_ga.SetMutationRate(0.2f);
				this->_ga.SaveFittestNetwork(this->_token);
				//set the rest of the population controllers to the best one
				for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
					nnplayer.Restart();
				}
			}
			else {
				this->_ga.Solved();
			}
		}


		if (!_ga.isSolved()) {
			this->_ga.SaveFittestNetwork(this->_token);
			this->_ga.NextGeneration();
			std::vector<NeuralNetwork>& gapop = this->_ga.GetPopulation();
			for (int i = 0; i < (int)gapop.size(); i++) {
				NNControlledPlayer& nnplayer = this->_playerPopulation.at(i);
				nnplayer.Restart();
				nnplayer.SetNNController(&gapop.at(i));
			}
		}
		else {
			this->_ga.SaveFittestNetwork(this->_token);
			std::cout << "Player has beaten the game, well done!\n" << std::endl;
			this->_data->stateMachine.PopState();
		}

	}
	this->_data->window.clear(sf::Color::White);
	if (this->_display) {
		_info.setString("Speed: " + std::to_string(this->_data->gameSpeedMultiplier) + "x , TTL: " + std::to_string(_ttlClock.getElapsedTime().asSeconds()) + " / " + std::to_string(_ttl/this->_data->gameSpeedMultiplier));
		_info.setPosition(this->_data->camera.GetCameraBox().left, this->_data->camera.GetCameraBox().top);

		this->_levels.at(this->_currentLevel).Draw();
		this->_data->gameObjectManager.Draw(dt);

		this->_data->window.draw(_info);
	}
	this->_data->window.display();
}

