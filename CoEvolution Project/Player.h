#pragma once
#include "Game.h"
#include "Level.h"
#include "IEntity.h"

class Player : virtual public IEntity {

public:
	Player(GameDataRef data, Level** level, sf::Vector2f wh);
	~Player(){}

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	void Jump();
	void Left();
	void Right();
	void Stop();
	virtual void Die();
	void Respawn();
	void Restart();
	virtual void Finish();

	void SetProgress(float progress);
	const float& GetProgress() const;

	void SetColor(sf::Color color);

	bool IsAlive();

	float PercentageOfLevelCompleted();

	sf::Vector2f& GetPosition() {
		return _position;
	}
	
	bool _holdingJump;

protected:
	GameDataRef _data;
	sf::Sprite _sprite;
	Level** _level;

	float _speed, _jumpVelocity;
	int _direction = 0, _lives = 3, _currentCheckpoint = 0;
	bool _falling, _jumping, _jump, _grounded;
	float _progress = 0.0f;
};