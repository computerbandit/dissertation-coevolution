#include "Coin.h"

Coin::Coin(GameDataRef data, sf::Vector2f pos): _data(data)
{
	AssetManager::rescale(this->_sprite, ZOOM_FACTOR);
	this->_position = pos;
	this->_sprite.setPosition(this->_position);
	this->_animController = new AnimationController(this->_sprite);
	std::vector<unsigned int> animIds = std::vector<unsigned int>();
	animIds.push_back(COIN_IDLE);
	animIds.push_back(COIN_COLLECTED);
	this->_animController->mapAnimations(&this->_data->assetManager, animIds);
	this->_animController->nextAnimation(COIN_IDLE, true, false);
}

Coin::~Coin()
{
	delete this->_animController;
}

void Coin::init()
{
	//this->_velocity = sf::Vector2f(10.0f, 10.0f);
	//this->activate();
}

void Coin::update(float dt)
{
	if (this->_collected && this->_animController->hasCurrentAnimTimedOut()) {
		destory();
	}
}

void Coin::draw(float dt)
{
	this->_animController->update();
	this->_data->window.draw(this->_sprite);
}

void Coin::collect()
{
	this->_collected = true;
	this->_animController->nextAnimation(COIN_COLLECTED, false, false);
}
