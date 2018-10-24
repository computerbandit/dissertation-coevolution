#pragma once
#include "SFML/Graphics.hpp"

class Camera {
public:
	Camera() {}
	Camera(sf::RenderWindow* window, float width, float height);
	~Camera() {}

	void Restore();
	void Update(sf::Vector2f point);

	const sf::FloatRect GetCameraBox();
private:
	sf::RenderWindow* _window;
	sf::View _view;
};
