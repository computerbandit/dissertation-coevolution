#pragma once
#include "SFML/Graphics.hpp"

class Camera {
public:
	Camera() {}
	Camera(sf::RenderWindow* window, float width, float height);
	~Camera() {}

	void Restore();
	void Update(sf::Vector2f point);

private:
	sf::RenderWindow* _window;
	sf::View _view;
	sf::Vector2f _offset;
	sf::Vector2f _dampening;
};
