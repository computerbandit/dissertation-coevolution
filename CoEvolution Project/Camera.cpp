#include "Camera.h"
#include <iostream>

Camera::Camera(sf::RenderWindow* window, sf::Vector2u wh, sf::Vector2f offset): _offset(offset)
{
	this->_window = window;
	this->setSize(sf::Vector2f(wh));
	this->_view.setCenter(_offset);
	this->_window->setView(_view);
}

void Camera::restore()
{
	this->_window->setView(this->_window->getDefaultView());
}

void Camera::setCenter(sf::Vector2f point)
{
	_view.setCenter(point);
	this->_window->setView(_view);
}

void Camera::setSize(sf::Vector2f wh)
{
	this->_wh = wh;
	this->_view.setSize(_wh.x, _wh.y);
	this->_window->setView(_view);
}

void Camera::zoom(float zoomfactor)
{
	this->_zoom += zoomfactor - 1.0f;
	sf::Vector2f zoom = this->_wh * zoomfactor;
	this->setSize(zoom);
}

void Camera::resetZoom()
{
	this->setSize(sf::Vector2f(this->_window->getSize()));
	this->_zoom = 1.0f;
}

void Camera::update(sf::Vector2f point)
{
	//camera gets locked when going backwards so that when the player respawns the cam goes to the char faster
	float dampx = (point.x - (_view.getCenter().x - _view.getSize().x / 4));
	float dampy = (point.y - _view.getCenter().y); 

	if (dampx >= 0) {
		dampx /= 300;
	}
	else {
		dampx /= 10;
	}
	_view.move(dampx, dampy);

	this->_window->setView(_view);
}

const sf::FloatRect Camera::getCameraBox()
{
	return  sf::FloatRect(_view.getCenter() - (_view.getSize() / 2.0f), _view.getSize());
}

void Camera::resize(sf::Event event){

	int newWidth = (event.size.width >= 1300)? 1300 : event.size.width;

	int newHeight = (newWidth/ASPECT_RATIO_WIDTH) * ASPECT_RATIO_HEIGHT;
	//maintain aspect ratio
	_window->setSize(sf::Vector2u(newWidth, newHeight));
	
	//sf::FloatRect visibleArea(0, 0, (float)newWidth, (float)newHeight);
	//_view = sf::View(visibleArea);
	//_window->setView(_view);
}
