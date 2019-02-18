#pragma once
#include "SFML/Graphics.hpp"
class IEntity{
public:

	~IEntity() {}

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;

	bool isActive() {
		return _active;
	}

	bool toBeDestroyed() {
		return _destroy;
	}

	void destory() {
		deactivate();
		_destroy = true;
	}

	void deactivate() {
		_active = false;
	}
	void activate() {
		_active = true;
	}

protected:
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	bool _active = true;
	bool _destroy = false;
};