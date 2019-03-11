#include "TestNetworkState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>
#include <string>
#include <windows.h>

TestNetworkState::TestNetworkState(GameDataRef data) :_data(data)
{
	this->_player = nullptr;
	this->_currentLevel = 0;
	this->_levels = std::vector<Level>();
}

void TestNetworkState::init()
{
	//load the levels in the order to play them;
	_levels.push_back(Level(_data, VALIDATION_LEVEL_PATH"lvl-9", LEVEL_1_TIME));
	//_levels.push_back(Level(Noise::GenHeightMap(sf::Vector2i(100, 4), 3, 2, 1), _data, "levelgentest-1", 15.0f));


	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Network\0*.net\0Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a Network!";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		_player = new NNControlledPlayer(this->_data, &_levels, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2), new NeuralNetwork(static_cast<std::string>(filename)));

		this->_data->gameObjectManager.addEntity(_player, PLAYER_LAYER);
	}
	else
	{
		switch (CommDlgExtendedError())
		{
			default: 
				std::cout << "You cancelled.\n";
				this->_data->stateMachine.popState();
		}
	}

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
	this->_data->camera.zoom(1.2f);
}

void TestNetworkState::cleanup()
{
	this->_data->camera.restore();
	this->_data->gameObjectManager.clearEntities();
	this->_levels.clear();
	delete this->_player;
}

void TestNetworkState::handleEvents()
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
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (_currentLevel + 1 < (int)this->_levels.size()) {
					this->_currentLevel++;
					this->_player->nextLevel();
					this->_player->restart();
				}
				else {
					this->_data->stateMachine.popState();
				}
			}

		}
	}
}

void TestNetworkState::update(float dt)
{
	if (_player != nullptr) {
		_player->getNetworkController()->run(_player->controllersViewOfLevel());
		std::vector<float> output = _player->getNetworkController()->getOutput();
		//given the outputs of the network 

		if (output.size() == 2) {
			//go left 
			if (output.at(0) > 0.9f) {
				_player->right();
			}
			else if (output.at(0) < -0.9f) {
				_player->left();
			}
			else {
				_player->stop();
			}
			// Jump
			if (output.at(1) > 0.9f) {
				_player->jump();
			}
			else {
				_player->stopJumping();
			}
		}
		if (_player->isFinished()) {
			if (_currentLevel + 1 < (int)this->_levels.size()) {
				this->_currentLevel++;
				this->_player->nextLevel();
				this->_player->restart();
			}
			else {
				this->_data->stateMachine.popState();
			}
		}
	}
	this->_data->gameObjectManager.update(dt);
}

void TestNetworkState::draw(float dt)
{
	this->_data->window.clear(sf::Color(234, 212, 170, 255));
	if (this->_player != nullptr) {
		this->_data->camera.update(_player->getSpriteCenterPosition());
	}
	this->_levels.at(this->_currentLevel).draw();
	this->_data->gameObjectManager.draw(dt);
	this->_data->window.display();
}
