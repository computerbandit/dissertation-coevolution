#pragma once
#include "SFML/Graphics.hpp"
class IEntity{
public:

	~IEntity() {}

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(float dt) = 0;

	bool IsActive() {
		return _active;
	}
	void Deactivate() {
		_active = false;
	}
	void Activate() {
		_active = true;
	}

protected:
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	bool _active = true;
};