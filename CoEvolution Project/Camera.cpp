#include "Camera.h"
#include <iostream>

void Camera::Init(sf::RenderWindow* window, float width, float height)
{
	this->_window = window;
	this->_offset = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
	this->_view.setSize(width, height);
	this->_view.setCenter(200, 300);
	this->_view.setViewport(sf::FloatRect(0, 0, 1, 1));
	this->_window->setView(_view);
}

void Camera::Restore()
{
	this->_window->setView(this->_window->getDefaultView());
}

void Camera::Update(sf::Vector2f point)
{
	float dampx = ((point.x - (_view.getCenter().x - 200))/100)*9.0f;
	_view.move(dampx, 0);
	this->_window->setView(_view);
}