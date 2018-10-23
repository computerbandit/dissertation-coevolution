#include "Player.h"
#include "Tile.h"
#include <iostream>

Player::Player(GameDataRef data, sf::Vector2f pos): _data(data)
{
	this->_speed = 20.0f;
	this->_jumpVelocity = 20.0f;
	this->_position = pos;
	this->Init();
}


void Player::Init()
{
	_jump = false;
	_falling = true;
	_jumping = false;
	_holdingJump = false; 
	_grounded = false;

	this->_rect = sf::RectangleShape(sf::Vector2f(30, 60));
	this->_velocity = sf::Vector2f(0.0f, 0.0f);
	this->_rect.setPosition(this->_position);
	this->_rect.setFillColor(sf::Color::Blue);

}

void Player::Update(float dt)
{
	if (_jump) {
		_jump = false;
		_grounded = false;
		_jumping = true;
		this->_velocity.y = -_jumpVelocity;
	}

	if (_falling && this->_velocity.y == 0.0f) {
		_grounded = true;
		_falling = false;
		_jumping = false;
	}else if ((_grounded || _jumping) && this->_velocity.y > 0.0f) {
		_grounded = false;
		_falling = true;
		_jumping = false;
	}else if (_grounded && this->_velocity.y < 0.0f) {
		_grounded = false;
		_falling = false;
		_jumping = true;
	}


	if (_falling){
		this->_velocity.y += this->_data->gravity.y * (2.5f) * dt;
	}
	else if(_jumping && !_holdingJump){
		this->_velocity.y += this->_data->gravity.y * (2.0f) * dt;
	}
	else {
		this->_velocity.y += this->_data->gravity.y * dt;
	}
	if (!_direction == 0) {
		this->_velocity.x = _speed * _direction;
	}
	if (!_grounded && this->_velocity.x != 0.0f) {
		this->_velocity.x -= this->_velocity.x * (0.1f);
	}
	else {
		this->_velocity.x -= this->_velocity.x * (0.35f);
	}


	//check collision for each tile in the collision map of the current level
	
	for (Tile* tile : this->_data->gameObjectManager.CollisionMap()) {
		std::cout << tile->GetSprite().getLocalBounds().left << tile->GetSprite().getLocalBounds().top << tile->GetSprite().getLocalBounds().width << tile->GetSprite().getLocalBounds().height << std::endl;
		if (_rect.getLocalBounds().intersects(tile->GetSprite().getLocalBounds())) {
			this->_velocity.y *= 0;
			this->_velocity.x *= 0;
		}
	}
	

	this->_position.y += this->_velocity.y * dt;
	this->_position.x += this->_velocity.x * dt;
	
	this->_rect.setPosition(this->_position);
}

void Player::Draw(float dt)
{
	this->_data->window.draw(this->_rect);
}

void Player::Jump()
{
	_holdingJump = true;
	if (_grounded) {
		_jump = true;
	}
}

void Player::Left()
{
	_direction = -1;
}

void Player::Right()
{
	_direction = 1;
}

void Player::Stop()
{
	_direction = 0;
}



