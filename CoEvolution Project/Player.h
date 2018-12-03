#pragma once
#include "Game.h"
#include "Level.h"
#include "IEntity.h"

class Player : virtual public IEntity {

public:
	Player(GameDataRef data, std::vector<Level>& levels, int& currentLevel, sf::Vector2f wh);
	~Player();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	float PercentageOfLevelCompleted();
	bool IsAlive();
	void Jump();
	void StopJumping();
	void Left();
	void Right();
	void Stop();
	virtual void Die();
	void Respawn();
	virtual void Restart();
	virtual void Finish();
	virtual void NextLevel();

	void SetProgress(float progress);
	void SetColor(sf::Color color);
	const float& GetProgress() const;
	const sf::Vector2f& GetPosition() const;

protected:
	GameDataRef _data;
	sf::Sprite _sprite;
	std::vector<Level>& _levels;
	int& _currentLevel;

	float _speed, _jumpVelocity;
	int _direction = 0, _lives = 3, _currentCheckpoint = 0;
	bool _falling, _jumping, _holdingJump, _jump, _grounded, _finished, _exit;
	float _progress = 0.0f;
};