#include "TrainNetworkState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include "DEFINITIONS.h"
#include "TestNetworkState.h"

TrainNetworkState::TrainNetworkState(GameDataRef data, float timetolive, float speedMultiplier, bool display): _data(data), _display(display), _ttl(timetolive)
{
	_ga = NeuralNetworkGA(NeuralNetwork::GeneratePopulation(DEFUALT_TRAINNING_POPULATION_SIZE, {INPUT_LAYER_SIZE, 4, 4, 3}), 0.5f);
	this->_levels = std::vector<Level>();
	this->_data->gameSpeedMultiplier = speedMultiplier;
}

void TrainNetworkState::Init()
{

	//load the texturesheet
	this->_data->assetManager.LoadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));

	//load the levels in the order to play them;
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	//_levels.push_back(Level(_data, TRAINNING_LEVEL_2));

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
	_ttl = 0.1f;
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
				this->_data->gameSpeedMultiplier = 3.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				this->_data->gameSpeedMultiplier = 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				this->_data->gameSpeedMultiplier = 5.0f;
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				//stop the  sim
				std::cout << "\ntraining sim stopped by user" << std::endl;
				//return to menu
				this->_data->stateMachine.PushState(StateRef(new MainMenuState(this->_data)));
			}
		}

		
	}
}

void TrainNetworkState::Update(float dt)
{
	//check all the players have died then eval them
	bool stillAlive = false;
	int numAlive = 0;
	for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
		if (!nnplayer.Finished()) {
			numAlive++;
			//update the player based on the nn that controls it
			NeuralNetwork* controller = nnplayer.GetNetworkController();
			//need to get a set of inputs from the ray cast info from each of the players

			std::vector<float> inputs = nnplayer.ConrollersViewOfLevel(CONTROLLER_TILES_VIEW);
			controller->Run(inputs);
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
			stillAlive = true;
		}
	}

	//updating all nncontrolled player objects in the game
	this->_data->gameObjectManager.Update(dt);
	//find the nnplayer that has made the most progess if we are displaying then set th cameras postition to the best controller
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
		if(_display) {
			bestController->SetColor(sf::Color::Red);
			this->_data->camera.Update(bestController->GetPosition());
		}
	}
	else {
		if (_display) {
			this->_data->camera.Update(this->_levels.at(this->_currentLevel).GetCheckpoint(0));
		}
	}

	if (this->_clock.getElapsedTime().asSeconds() > (this->_ttl/this->_data->gameSpeedMultiplier) || !stillAlive) {
		this->_clock.restart();
			//set the fitnessScore for the each of the controllers now that they are all updated position wise
		for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
			nnplayer.GetNetworkController()->SetFitnessScore(nnplayer.GetProgress());
			if (this->EvaluateNNControlledPlayer(nnplayer)) {
				
				if (_currentLevel + 1 < this->_levels.size()) {

					std::cout << "\n Level Completed" << std::endl;
					this->_currentLevel++;
					for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
						nnplayer.Restart();
					}
				}
				else {
					this->_ga.Solved();
					break;
				}
				
			}
		}
		this->_ga.EvalutePopulation();

		std::cout << "Generation [" << this->_ga.GetGeneration() << "] -> Percentage Progress: " << this->_ga.AverageFitness() << "% average, " << this->_ga.FittestNetwork().GetFitnessScore() << "% best controller" << "\r";

		if(!_ga.isSolved()) {
			this->_ga.SaveFittestNetwork();
			this->_ga.NextGeneration();
			std::vector<NeuralNetwork>& gapop = this->_ga.GetPopulation();
			for (int i = 0; i < (int)gapop.size(); i++) {
				NNControlledPlayer& nnplayer = this->_playerPopulation.at(i);
				nnplayer.Restart();
				nnplayer.SetNNController(&gapop.at(i));
			}
		}
		else {
			this->_ga.SaveFittestNetwork();
			
			std::cout << "Player has beaten the game, well done!\n" << std::endl;
			this->_data->stateMachine.PopState();
		}
	}
}

void TrainNetworkState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::White);
	if (this->_display) {
		_info.setString("Speed: " + std::to_string(this->_data->gameSpeedMultiplier) + "x , TTL: " + std::to_string(_clock.getElapsedTime().asSeconds()) + " / " + std::to_string(_ttl));
		_info.setPosition(this->_data->camera.GetCameraBox().left, this->_data->camera.GetCameraBox().top);

		this->_levels.at(this->_currentLevel).Draw();
		this->_data->gameObjectManager.Draw(dt);

		this->_data->window.draw(_info);
	}
	this->_data->window.display();
}

bool TrainNetworkState::EvaluateNNControlledPlayer(NNControlledPlayer& nnplayer)
{
	if (nnplayer.GetProgress() >= 100.0f) {
		return true;
	}
	else {
		return false;
	}

}
