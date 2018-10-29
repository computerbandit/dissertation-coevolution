#pragma once
#include "Game.h"
#include "Level.h"
#include "IEntity.h"

class Player : virtual public IEntity {

public:
	Player(GameDataRef data, Level** level, sf::FloatRect box);
	~Player(){}

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	void Jump();
	void Left();
	void Right();
	void Stop();
	void Die();
	void Respawn();
	void Finish();

	sf::Vector2f& GetPosition() {
		return _position;
	}
	
	bool _holdingJump;

private:
	GameDataRef _data;
	sf::Sprite _sprite;
	Level** _level;

	float _speed, _jumpVelocity;
	int _direction = 0, _lives = 3, _currentCheckpoint = 0;
	bool _falling, _jumping, _jump, _grounded;
};