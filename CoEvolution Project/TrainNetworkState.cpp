#include "TrainNetworkState.h"
#include <string>
#include <iostream>
#include "DEFINITIONS.h"

TrainNetworkState::TrainNetworkState(GameDataRef data): _data(data)
{
	_ga = NeuralNetworkGA(NeuralNetwork::GeneratePopulation(20, {1, 3}), 0.5f);
	this->_level = new Level(_data);
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
}

void TrainNetworkState::Cleanup()
{
	this->_data->camera.Restore();
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
	}
}

void TrainNetworkState::Update(float dt)
{
	//make sure that the generation dies after a certain time like 20 secs;
	
	//check all the players have died then eval them
	bool stillAlive = false;
	for (NNControlledPlayer& nnplayer : _playerPopulation) {
		if (!nnplayer.Finished()) {
			//update the player based on the nn that controls it
			NeuralNetwork* controller = nnplayer.GetNetworkController();
			//need to get a set of inputs;
			std::vector<float> inputs = { 1.0f };
			controller->Run(inputs);
			std::vector<float> output = controller->GetOutput();
			//given the outputs of the network 
			
			// go left
			if (output.at(0) > 0.9f) {
				nnplayer.Right();
			}
			// go right
			if (output.at(1) > 0.9f) {
				nnplayer.Right();
			}
			// Jump
			if (output.at(2) > 0.9f) {
				nnplayer.Jump();
			}
			stillAlive = true;
		}
	}

	this->_data->gameObjectManager.Update(dt);

	sf::Vector2f cameraFollow(0.0f, 0.0f);


	if (this->_clock.getElapsedTime().asSeconds() > 2.5f || !stillAlive) {
		
		this->_clock.restart();
		
		float mostProgress = 0.0f;
		//set the fitnessScore for the each of the controllers now that they are all updated position wise
		for (NNControlledPlayer& nnplayer : _playerPopulation) {
			nnplayer.GetNetworkController()->SetFitnessScore(nnplayer.GetProgress());

			if (this->EvaluateNNControlledPlayer(nnplayer)) {
				_ga.Solved();
				break;
			}
			if (nnplayer.GetProgress() > mostProgress) {
				cameraFollow = nnplayer.GetPosition();
			}
		}
		_ga.EvalutePopulation();

		std::cout << "Generation [" << _ga.GetGeneration() << "] -> Percentage Progress: " << _ga.AverageFitness() << "% average, " << _ga.FittestNetwork().GetFitnessScore() << "% best controller" << "\r";


		//set the camera to the position of the top contorller;


		if(!_ga.isSolved()) {
			_ga.SaveFittestNetwork();
			_ga.NextGeneration();
			this->_data->gameObjectManager.ClearEntities();
			_playerPopulation = std::vector<NNControlledPlayer>();
			std::vector<NeuralNetwork>& gapop = _ga.GetPopulation();
			for (int i = 0; i < (int)_ga.GetPopulation().size(); i++) {
				_playerPopulation.push_back(NNControlledPlayer(_data, &_level, sf::Vector2f(16, 16), &gapop.at(i)));
			}
			for (auto& n : _playerPopulation) {
				this->_data->gameObjectManager.AddEntity(&n);
			}
		}
	}

	this->_data->camera.Update(cameraFollow);
}

void TrainNetworkState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::White);
	this->_level->Draw();
	this->_data->gameObjectManager.Draw(dt);
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
