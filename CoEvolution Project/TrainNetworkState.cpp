#include "TrainNetworkState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include "DEFINITIONS.h"

TrainNetworkState::TrainNetworkState(GameDataRef data, float timetolive, float speedMultiplier, bool display): _data(data), _display(display), _ttl(timetolive)
{
	_ga = NeuralNetworkGA(NeuralNetwork::GeneratePopulation(DEFUALT_TRAINNING_POPULATION_SIZE, {2, 3, 4, 3}), 0.1f);
	this->_level = new Level(_data);
	this->_data->gameSpeedMultiplier = speedMultiplier;
}

void TrainNetworkState::Init()
{

	//load the texturesheet
	this->_data->assetManager.LoadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));
	this->_data->assetManager.LoadTexturesheet(PLATFORMS, PLATFORM_SHEET, sf::Vector2u(16, 4));

	//load the level text file and set up tiles.
	this->_level->LoadLevel(1);

	this->_data->assetManager.LoadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(16, 16));

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));

	srand(time(0));
	_playerPopulation = std::vector<NNControlledPlayer>();
	std::vector<NeuralNetwork>& gapop = _ga.GetPopulation();
	for (int i = 0; i < (int)_ga.GetPopulation().size(); i++) {
		_playerPopulation.push_back(NNControlledPlayer(_data, &_level, sf::Vector2f(16, 16), &gapop.at(i)));
	}
	for (auto& n : _playerPopulation) {
		this->_data->gameObjectManager.AddEntity(&n);
	}

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
			this->_data->window.close();
			this->Cleanup();
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.Resize(event);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			//stop the  sim
			std::cout << "\ntraining sim stopped by user" << std::endl;
			//return to menu
			this->_data->stateMachine.PushState(StateRef(new MainMenuState(this->_data)));
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



			std::vector<float> inputs = { 1.0f , 0.5,1.0f,0.0f};
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
		bestController->SetColor(sf::Color::Red);
		this->_data->camera.Update(bestController->GetPosition());
	}
	else {
		this->_data->camera.Update(_level->GetCheckpoint(0));
	}

	if (this->_clock.getElapsedTime().asSeconds() > (this->_ttl/this->_data->gameSpeedMultiplier) || !stillAlive) {
		this->_clock.restart();
			//set the fitnessScore for the each of the controllers now that they are all updated position wise
		for (NNControlledPlayer& nnplayer : this->_playerPopulation) {
			nnplayer.GetNetworkController()->SetFitnessScore(nnplayer.GetProgress());
			if (this->EvaluateNNControlledPlayer(nnplayer)) {
				this->_ga.Solved();
				break;
			}
		}
		this->_ga.EvalutePopulation();

		std::cout << "Generation [" << this->_ga.GetGeneration() << "] -> Percentage Progress: " << this->_ga.AverageFitness() << "% average, " << this->_ga.FittestNetwork().GetFitnessScore() << "% best controller" << "\r";

		if(!_ga.isSolved()) {
			this->_ga.SaveFittestNetwork();
			this->_ga.NextGeneration();
			this->_data->gameObjectManager.ClearEntities();
			bestController = nullptr;
			this->_playerPopulation = std::vector<NNControlledPlayer>();
			std::vector<NeuralNetwork>& gapop = this->_ga.GetPopulation();
			for (int i = 0; i < (int)this->_ga.GetPopulation().size(); i++) {
				this->_playerPopulation.push_back(NNControlledPlayer(this->_data, &_level, sf::Vector2f(16, 16), &gapop.at(i)));
			}
			for (auto& n : this->_playerPopulation) {
				this->_data->gameObjectManager.AddEntity(&n);
			}
		}
		else {
			this->_ga.SaveFittestNetwork();
			std::cout << "\n Level Completed" << std::endl;
			this->_data->gameSpeedMultiplier = 1.0f;
			this->_data->stateMachine.PushState(StateRef(new MainMenuState(this->_data)));
		}
	}
}

void TrainNetworkState::Draw(float dt)
{
	if (this->_display) {
		this->_data->window.clear(sf::Color::White);
		this->_level->Draw();
		this->_data->gameObjectManager.Draw(dt);
		this->_data->window.display();
	}
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
