#include "Player.h"
#include "Tile.h"
#include "MainMenuState.h"
#include "DEFINITIONS.h"
#include <iostream>

Player::Player(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh) : _data(data), _levels(levels), _currentLevel(currentLevel)
{
	this->_speed = 300.0f;
	this->_jumpVelocity = 450.0f;
	this->_sprite.setTexture(this->_data->assetManager.GetTexturesheet(PLAYER).GetTexture(0));
	AssetManager::Rescale(_sprite, wh);
	this->_sprite.setColor(sf::Color::Blue);
	this->Init();
}

Player::~Player()
{

}


void Player::Init()
{
	_jump = false;
	_falling = true;
	_jumping = false;
	_holdingJump = false; 
	_grounded = false;
	_finished = false;
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
	int num_steps = 4;

	for (int i = 0; i < num_steps; i++) {
		oldpos = sf::Vector2f(this->_position);
		this->_position.x += this->_velocity.x * (dt/num_steps);
		_sprite.setPosition(this->_position);
		bool collision = this->_levels.at(this->_currentLevel).Collision(_sprite.getGlobalBounds());
		if (collision) {
			this->_position = oldpos;
			_sprite.setPosition(this->_position);
		}

		oldpos = sf::Vector2f(this->_position);
		this->_position.y += this->_velocity.y * (dt/num_steps);
		_sprite.setPosition(this->_position);
		collision = this->_levels.at(this->_currentLevel).Collision(_sprite.getGlobalBounds());
		if (collision) {
			this->_position = oldpos;
			this->_velocity.y = 0;
			_sprite.setPosition(this->_position);
		}
	}

	//if the player goes under the map then they die;
	if (this->_position.y > 700) {
		this->Die();
	}

	//if the next checkpoint is the of the level then when the player passes it they win finish
	if (this->_levels.at(this->_currentLevel).LastCheckpoint(_currentCheckpoint + 1)) {
		if (this->_position.x >= this->_levels.at(this->_currentLevel).GetCheckpoint(_currentCheckpoint + 1).x) {
			//player beat the level.
			this->Finish();
		}
	}
	//if it is no the last check point then when the player passes it, it just sets that as the current checkpoint
	else if (this->_position.x >= this->_levels.at(this->_currentLevel).GetCheckpoint(_currentCheckpoint + 1).x ) {
		_currentCheckpoint++;
	}

	this->_sprite.setPosition(this->_position);
	this->SetProgress(PercentageOfLevelCompleted());
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

void Player::StopJumping()
{
	this->_holdingJump = false;
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
	if (_lives <= 0) {
		this->Restart();
	}
	else {
		this->Respawn();
	}
}

void Player::Respawn()
{
	this->_position = this->_levels.at(this->_currentLevel).GetCheckpoint(this->_currentCheckpoint);
	this->_velocity = sf::Vector2f(0.0f, 0.0f);
}

void Player::Restart()
{
	this->_lives = 3;
	this->_currentCheckpoint = 0;
	this->Respawn();
}

void Player::Finish()
{
	//player has finished the level...
	this->_finished = true;
	//could start a clock and have the next level after like 2 secs;
	//and display you finished in overlay text or something;
	this->NextLevel();
	this->Restart();
}

void Player::NextLevel()
{
	//go to the next level or finish the game as the player beat the last level
	if (_currentLevel + 1 < this->_levels.size()) {
		this->_currentLevel++;
	}
	else {
		std::cout << "Player has beaten the game, well done!\n" << std::endl;
		this->_data->stateMachine.PopState();
	}
	this->_currentCheckpoint = 0;
}

void Player::SetProgress(float progress)
{
	this->_progress = progress;
}

const float & Player::GetProgress() const
{
	return this->_progress;
}

void Player::SetColor(sf::Color color)
{
	this->_sprite.setColor(color);
}

bool Player::IsAlive()
{
	return (_lives > 0);
}

float Player::PercentageOfLevelCompleted()
{
	//find the position of the player and compare it to the position of the final checkpoint
	float start = this->_levels.at(this->_currentLevel).GetCheckpoint(0).x;
	float end = this->_levels.at(this->_currentLevel).GetFinishFlagPosition().x;
	float ratio = (this->_position.x - start) / (end - start);
	float percentage = ratio * 100.0f;
	if (percentage > 100.0f || percentage < 0.0f) {
		percentage = std::max(0.0f, std::min(percentage, 100.0f));
	}
	return  percentage;
}

const sf::Vector2f & Player::GetPosition() const
{
	return this->_position;
}



