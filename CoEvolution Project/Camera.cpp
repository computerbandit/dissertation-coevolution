#include "Camera.h"
#include <iostream>

Camera::Camera(sf::RenderWindow* window, sf::Vector2u wh, sf::Vector2f offset): _offset(offset)
{
	this->_window = window;
	this->_view.setSize((float)wh.x, (float)wh.y);
	this->_view.setCenter(_offset);
	this->_window->setView(_view);
}

void Camera::restore()
{
	this->_window->setView(this->_window->getDefaultView());
}

void Camera::update(sf::Vector2f point)
{
	if (this->getCameraBox().contains(point)) {
		float dampx = ((point.x - (_view.getCenter().x - _view.getSize().x / 4)) / 100)*6.0f;
		float dampy = 0;//((point.y - (_view.getCenter().y + _view.getSize().y / 8)) / 100)*20.0f;
		//could do some sort of zooming out while the player is moving but that is hard :(
		_view.move(dampx, dampy);
	}
	else {
		_view.setCenter(point);
	}
	this->_window->setView(_view);
}

const sf::FloatRect Camera::getCameraBox()
{
	return  sf::FloatRect(_view.getCenter() - (_view.getSize() / 2.0f), _view.getSize());
}

void Camera::resize(sf::Event event){

	int newWidth = event.size.width;
	int newHeight = (newWidth/ASPECT_RATIO_WIDTH) * ASPECT_RATIO_HEIGHT;
	//maintain aspect ratio
	_window->setSize(sf::Vector2u(newWidth, newHeight));
	
	sf::FloatRect visibleArea(0, 0, (float)newWidth, (float)newHeight);
	_view = sf::View(visibleArea);
	_window->setView(_view);
}
