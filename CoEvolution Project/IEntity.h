#pragma once
#include "SFML/Graphics.hpp"
class IEntity{
public:

	IEntity() {}
	~IEntity() {}

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(float dt) = 0;


protected:
	sf::Vector2f _velocity;
	sf::Vector2f _position;
};