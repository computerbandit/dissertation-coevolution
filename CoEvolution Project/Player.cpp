#include "Player.h"
#include "Tile.h"
#include "MainMenuState.h"
#include "DEFINITIONS.h"
#include <iostream>

Player::Player(GameDataRef data, Level** level, sf::Vector2f wh): _data(data), _level(level)
{
	this->_speed = 300.0f;
	this->_jumpVelocity = 450.0f;
	this->_sprite.setTexture(this->_data->assetManager.GetTexturesheet(PLAYER).GetTexture(0));
	AssetManager::Rescale(_sprite, wh);
	this->_sprite.setColor(sf::Color::Blue);
	this->Init();
}


void Player::Init()
{
	_jump = false;
	_falling = true;
	_jumping = false;
	_holdingJump = false; 
	_grounded = false;
	this->Respawn();
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

	if (this->_velocity.y > 1000.0f) {
		this->_velocity.y = 1000.0f;
	}
	else if (this->_velocity.y < -1000.0f){
		this->_velocity.y = -1000.0f;
	}


	//collsison detection in 5 substeps with wall sliding
	sf::Vector2f oldpos;
	int num_steps = 5;

	for (int i = 0; i < num_steps; i++) {
		oldpos = sf::Vector2f(this->_position);
		this->_position.x += this->_velocity.x * (dt/num_steps);
		_sprite.setPosition(this->_position);
		Tile* collisionTile = (*_level)->Collision(_sprite.getGlobalBounds());
		if (collisionTile) {
			this->_position = oldpos;
			_sprite.setPosition(this->_position);
		}

		oldpos = sf::Vector2f(this->_position);
		this->_position.y += this->_velocity.y * (dt/num_steps);
		_sprite.setPosition(this->_position);
		collisionTile = (*_level)->Collision(_sprite.getGlobalBounds());
		if (collisionTile) {
			this->_position = oldpos;
			this->_velocity.y = 0;
			_sprite.setPosition(this->_position);
		}
	}

	//if the player goes under the map then they die;
	if (this->_position.y > 700) {
		this->Die();
	}

	if ((*_level)->LastCheckpoint(_currentCheckpoint + 1)) {
		if (this->_position.x >= (*_level)->GetCheckpoint(_currentCheckpoint + 1)->x) {
			//player beat the level.
			this->Finish();
		}
	}
	else if (this->_position.x >= (*_level)->GetCheckpoint(_currentCheckpoint + 1)->x ) {
		std::cout << "checkpoint hit!" << std::endl;
		_currentCheckpoint++;
	}

	this->_sprite.setPosition(this->_position);
}

void Player::Draw(float dt)
{
	this->_data->window.draw(this->_sprite);
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

void Player::Die()
{
	_lives--;
	if (_lives == 0) {
		this->_data->stateMachine.PushState(StateRef(new MainMenuState(_data)));
		this->Deactivate();
	}
	else {
		this->Respawn();
	}
}

void Player::Respawn()
{
	this->_position = *(*_level)->GetCheckpoint(this->_currentCheckpoint);
	this->_velocity = sf::Vector2f(0.0f, 0.0f);
}

void Player::Finish()
{
	//player has finished the level...
	/* evaluate the player with score and time and stuff.
	then load the next level after it has loaded or the player clicks a button or something
	idk get off my back man*/
	std::cout << "You won the level!" << std::endl;
	_currentCheckpoint = 0;
	(*_level)->LoadNextLevel();
	this->Respawn();
}



