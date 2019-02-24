#pragma once
#include <SFML/Graphics.hpp>
#define ASPECT_RATIO_WIDTH 4
#define ASPECT_RATIO_HEIGHT 3

class Camera {
public:
	Camera() {}
	Camera(sf::RenderWindow* window, sf::Vector2u wh, sf::Vector2f offset);
	~Camera() {}

	void restore();
	void setCenter(sf::Vector2f point);
	void setSize(sf::Vector2f wh);
	void zoom(float zoomfactor);
	void resetZoom();
	void update(sf::Vector2f point);

	const sf::FloatRect getCameraBox();

	void resize(sf::Event event);
private:
	sf::RenderWindow* _window = nullptr;
	sf::View _view;
	sf::Vector2f _offset;
	sf::Vector2f _wh;
	float _zoom = 1.0f;
};
