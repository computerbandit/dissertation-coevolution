#pragma once
#include "SFML/Graphics.hpp"
class IEntity {
public:

	~IEntity() {}

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;

	inline bool isActive() { return _active; }

	inline bool toBeDestroyed() { return _destroy; }

	inline void destory() {
		deactivate();
		_destroy = true;
	}

	inline void deactivate() { _active = false; }

	inline void activate() { _active = true; }

protected:
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	bool _active = true;
	bool _destroy = false;
};