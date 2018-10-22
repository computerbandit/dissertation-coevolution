#pragma once
#include "Game.h"
#include "IEntity.h"

class Player : virtual public IEntity {

public:
	Player(GameDataRef data, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));
	~Player();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;
	
	void Jump();
	void Left();
	void Right();
	void Stop();

	sf::Vector2f& GetPosition() {
		return _position;
	}
	
	bool _holdingJump;

private:
	GameDataRef _data;
	sf::RectangleShape _rect; //will be a sprite a some point
	sf::Sprite _sprite;

	float _speed = 400.0f;
	float _jumpVelocity = 300.0f;
	int _direction = 0;
	
	bool _falling, _jumping, _jump, _grounded;
};