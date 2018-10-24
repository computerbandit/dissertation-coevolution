#include "Camera.h"
#include <iostream>

Camera::Camera(sf::RenderWindow* window, float width, float height)
{
	this->_window = window;
	this->_view.setSize(width, height);
	this->_view.setCenter(width/2, height/4);
	this->_window->setView(_view);
}

void Camera::Restore()
{
	this->_window->setView(this->_window->getDefaultView());
}

void Camera::Update(sf::Vector2f point)
{
	if (this->GetCameraBox().contains(point)) {
		float dampx = ((point.x - (_view.getCenter().x - _view.getSize().x / 4)) / 100)*6.0f;
		float dampy = ((point.y - (_view.getCenter().y)) / 100)*20.0f;
		_view.move(dampx, dampy);
	}
	else {
		_view.setCenter(point);
	}
	this->_window->setView(_view);
}

const sf::FloatRect Camera::GetCameraBox()
{
	return  sf::FloatRect(_view.getCenter() - (_view.getSize() / 2.0f), _view.getSize());
}
