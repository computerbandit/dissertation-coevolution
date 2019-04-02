#include <sstream>
#include <iostream>
#include "SplashState.h"
#include "MainMenuState.h"
#include "../Framework/DEFINITIONS.h"

SplashState::SplashState(GameDataRef data) : _data(data)
{
	this->_loadAssets = true;
}

void SplashState::init()
{
	this->_data->assetManager.loadTexture("Splash State Background", SPLASH_STATE_BACKGROUND_PATH);
	_background.setTexture(this->_data->assetManager.getTexture("Splash State Background"));
	AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
}

void SplashState::cleanup()
{

}

void SplashState::handleEvents()
{
	sf::Event event;
	while (this->_data->window.pollEvent(event)) {
		if (sf::Event::Closed == event.type) {
			this->_data->window.close();
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			this->_data->stateMachine.pushState(StateRef(new MainMenuState(this->_data)));
		}
		if (sf::Event::Resized == event.type) {
			this->_data->camera.resize(event);
			AssetManager::rescale(_background, sf::Vector2f(this->_data->window.getSize()));
		}
	}
}

void SplashState::update(float dt)
{
	if (this->_loadAssets) {
		this->_data->assetManager.loadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(TEXTURE_SIZE, TEXTURE_SIZE));
		this->_data->assetManager.loadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(TEXTURE_SIZE, TEXTURE_SIZE));
		this->_data->assetManager.loadTexturesheet(EXTRA, EXTRA_SHEET, sf::Vector2u(TEXTURE_SIZE, TEXTURE_SIZE));

		//this->_data->assetManager.loadAnimationState(ERROR_TILE, &this->_data->assetManager.getTexturesheet(TILES), 62, 1, 10.0f, true);
		this->_data->assetManager.loadAnimationState(PLAYER_IDLE, &this->_data->assetManager.getTexturesheet(PLAYER), 8, 8, 500.0f, false);
		this->_data->assetManager.loadAnimationState(PLAYER_RUN, &this->_data->assetManager.getTexturesheet(PLAYER), 16, 8, 500.0f, false);
		this->_data->assetManager.loadAnimationState(COIN_IDLE, &this->_data->assetManager.getTexturesheet(EXTRA), 8, 8, 1000.0f, false);
		this->_data->assetManager.loadAnimationState(COIN_COLLECTED, &this->_data->assetManager.getTexturesheet(EXTRA), 16, 8, 500.0f, false);

		this->_loadAssets = false;
	}

	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME) {
		this->_data->stateMachine.pushState(StateRef(new MainMenuState(this->_data)));
	}
}

void SplashState::draw(float dt)
{
	this->_data->window.clear(sf::Color::Black);
	this->_data->window.draw(_background);
	this->_data->window.display();
}

void SplashState::loadingPhase()
{
	this->_data->assetManager.loadTexturesheet(TILES, TILE_SHEET, sf::Vector2u(TEXTURE_SIZE, TEXTURE_SIZE));
	this->_data->assetManager.loadTexturesheet(PLAYER, PLAYER_SHEET, sf::Vector2u(TEXTURE_SIZE, TEXTURE_SIZE));
}
