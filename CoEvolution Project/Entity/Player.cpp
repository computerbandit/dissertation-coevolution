#include "Player.h"
#include "Coin.h"
#include "../Framework/Tile.h"
#include "../States/MainMenuState.h"
#include "../Framework/DEFINITIONS.h"
#include <iostream>


Player::Player(GameDataRef data, std::vector<Level>* levels, sf::Vector2f wh) : _data(data), _levels(levels), _currentLevel(0)
{
	this->_speed = 300.0f;
	this->_jumpVelocity = 450.0f;
	this->_animController = new AnimationController(this->_sprite);
	std::vector<unsigned int> animIds = std::vector<unsigned int>();
	animIds.push_back(PLAYER_IDLE);
	animIds.push_back(PLAYER_RUN);
	this->_animController->mapAnimations(&this->_data->assetManager, animIds);
	this->_animController->nextAnimation(PLAYER_IDLE, true, false);
	AssetManager::rescale(this->_sprite, wh);
	this->_sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
	this->init();
}

Player::~Player()
{

}


void Player::init()
{

	this->respawn();
}

void Player::update(float dt)
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
	else {
		this->_animController->nextAnimation(PLAYER_IDLE, true, false);
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
	int num_steps = 3;

	for (int i = 0; i < num_steps; i++) {
		oldpos = sf::Vector2f(this->_position);
		this->_position.x += this->_velocity.x * (dt/num_steps);
		_sprite.setPosition(this->_position);
		bool collision = this->_levels->at(this->_currentLevel).collision(_sprite.getGlobalBounds());
		if (collision) {
			this->_position = oldpos;
			_sprite.setPosition(this->_position);
		}

		oldpos = sf::Vector2f(this->_position);
		this->_position.y += this->_velocity.y * (dt/num_steps);
		_sprite.setPosition(this->_position);
		collision = this->_levels->at(this->_currentLevel).collision(_sprite.getGlobalBounds());
		if (collision) {
			this->_position = oldpos;
			this->_velocity.y = 0;
			_sprite.setPosition(this->_position);
		}
	}

 
	for (Coin* coin : this->_data->gameObjectManager.collisionCheck<Coin>(this->_sprite.getGlobalBounds(), COIN_LAYER)) {
		if (!coin->isCollected()) {
			coin->collect();
			this->_score += 1;
		}
	}

	//if the player collides with a death tile then die
	if (this->_levels->at(this->_currentLevel).collisionWithTile(this->_sprite.getGlobalBounds(), BOTTOMOFLEVEL_TILE)) {
		this->die();
	}
	if (this->_levels->at(this->_currentLevel).collisionWithTile(this->_sprite.getGlobalBounds(), SPIKE_TILE)) {
		this->die();
	}

	//if the next checkpoint is the end of the level then when the player passes it they win finish
	if (this->_levels->at(this->_currentLevel).lastCheckpoint(_currentCheckpoint + 1)) {
		if (this->_position.x >= this->_levels->at(this->_currentLevel).getCheckpoint(_currentCheckpoint + 1).x) {
			//player beat the level.
			//fireworks and stop the player;
			this->finish();
		}
	}
	//if it is no the last check point then when the player passes it, it just sets that as the current checkpoint
	else if (this->_position.x >= this->_levels->at(this->_currentLevel).getCheckpoint(_currentCheckpoint + 1).x ) {
		_currentCheckpoint++;
	}

	this->_sprite.setPosition(this->_position);
	this->setProgress(percentageOfLevelCompleted());
}

void Player::draw(float dt)
{
	this->_animController->update();
	this->_data->window.draw(this->_sprite);
}


void Player::jump()
{
	_holdingJump = true;
	if (_grounded) {		
		_jump = true;
	}
}

void Player::stopJumping()
{
	this->_holdingJump = false;
}

void Player::left()
{
	this->_animController->nextAnimation(PLAYER_RUN, true, true);
	_direction = -1;
}

void Player::right()
{
	this->_animController->nextAnimation(PLAYER_RUN, true, false);
	_direction = 1;
}

void Player::stop()
{
	_direction = 0;
}

void Player::die()
{
	if (!_finished) {
		_lives--;
		this->deactivate();
		if (_lives <= 0) {
			this->restart();
		}
		else {
			this->respawn();
		}
	}
}

void Player::respawn()
{
	this->_position = this->_levels->at(this->_currentLevel).getCheckpoint(this->_currentCheckpoint);
	this->_velocity = sf::Vector2f(0.0f, 0.0f);
	this->activate();
}

void Player::restart()
{
	this->_lives = this->_startingLives;
	this->_currentCheckpoint = 0;
	this->_finished = false;
	this->respawn();
	this->_timer.restart();
}

void Player::finish()
{
	this->_levelTime = this->_timer.getElapsedTime().asSeconds();
	//player has finished the level...
	this->_finished = true;
	//could start a clock and have the next level after like 2 secs;
	//and display you finished in overlay text or something;
	
}

void Player::nextLevel()
{
	this->_currentLevel++;
}

void Player::selectLevel(int levelnum)
{
	this->_currentLevel = levelnum;
}

const int & Player::getCurrentLevel()
{
	return this->_currentLevel;
}

bool Player::isFinished()
{
	return this->_finished;
}

void Player::setProgress(float progress)
{
	this->_progress = progress;
}

const float & Player::getProgress() const
{
	return this->_progress;
}

void Player::setColor(sf::Color color)
{
	this->_sprite.setColor(color);
}

bool Player::isAlive()
{
	return (_lives > 0);
}

float Player::percentageOfLevelCompleted()
{
	//find the position of the player and compare it to the position of the final checkpoint
	float start = this->_levels->at(this->_currentLevel).getCheckpoint(0).x;
	float end = this->_levels->at(this->_currentLevel).getFinishFlagPosition().x;
	float ratio = (this->_position.x - start) / (end - start);
	float percentage = ratio * 100.0f;
	if (percentage > 100.0f || percentage < 0.0f) {
		percentage = std::max(0.0f, std::min(percentage, 100.0f));
	}
	return  percentage;
}

const sf::Vector2f & Player::getPosition() const
{
	return this->_position;
}

const sf::Vector2f Player::getSpriteCenterPosition() const
{
	float w = this->_sprite.getGlobalBounds().width;
	float h = this->_sprite.getGlobalBounds().height;
	return sf::Vector2f((w / 2) + this->_position.x, (h / 2) + this->_position.y);
}

const float & Player::getLevelTime() const
{
	return this->_levelTime;
}

const int & Player::getScore() const
{
	return this->_score;
}

void Player::setLevelPtr(std::vector<Level>* levels)
{
	this->_levels = levels;
}



