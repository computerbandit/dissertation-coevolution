#include "TestNetworkState.h"
#include "DEFINITIONS.h"

TestNetworkState::TestNetworkState(GameDataRef data) :_data(data)
{

	 _levels = std::vector<Level>();

}

void TestNetworkState::Init()
{
	//load the texturesheet
	this->_data->assetManager.LoadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(16, 16));

	//load the levels in the order to play them;
	_levels.push_back(Level(_data, TRAINNING_LEVEL_1));
	_levels.push_back(Level(_data, TRAINNING_LEVEL_2));

	this->_data->assetManager.LoadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(16, 16));
	_player = new NNControlledPlayer(this->_data, _levels, _currentLevel, sf::Vector2f(16, 16), new NeuralNetwork("Resources/networks/25_4_4_3-.txt"));

	this->_data->gameObjectManager.AddEntity(_player);

	this->_data->camera = Camera(&(this->_data->window), this->_data->window.getSize(), sf::Vector2f(0, 0));
}

void TestNetworkState::Cleanup()
{
	this->_data->camera.Restore();
	this->_data->gameObjectManager.ClearEntities();
}

void TestNetworkState::HandleEvents()
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
		}
	}
}

void TestNetworkState::Update(float dt)
{
	std::vector<float> inputs = _player->ConrollersViewOfLevel(CONTROLLER_TILES_VIEW);
	_player->GetNetworkController()->Run(inputs);
	std::vector<float> output = _player->GetNetworkController()->GetOutput();
	//given the outputs of the network 

	// go left
	if (output.at(0) > 0.9f) {
		_player->Right();
	}
	// go right
	if (output.at(1) > 0.9f) {
		_player->Left();
	}
	// Jump
	if (output.at(2) > 0.9f) {
		_player->Jump();
	}
	else {
		_player->StopJumping();
	} 

	this->_data->gameObjectManager.Update(dt);
	this->_data->camera.Update(_player->GetPosition());

}

void TestNetworkState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::White);
	this->_levels.at(this->_currentLevel).Draw();
	this->_data->gameObjectManager.Draw(dt);
	this->_data->window.display();
}
