#include "Tile.h"

Tile::Tile(GameDataRef data, int tileID, float tilesize, sf::Vector2i _mapsize, int index) : _tileID(tileID), _data(data)
{
	//set size 
	_rect = sf::RectangleShape(sf::Vector2f(tilesize, tilesize));
	//set position
	this->_position = sf::Vector2f((index % _mapsize.x)*tilesize, std::floor(index / _mapsize.x)*tilesize);
	_rect.setPosition(this->_position);
	switch (_tileID)
	{
	case 0:
		_rect.setFillColor(sf::Color::Red);
			break;
	case 1:
		_rect.setFillColor(sf::Color::Green);
		break;
	default:
		_rect.setFillColor(sf::Color::White);
		break;
	}
	_static = true;
}


void Tile::Init()
{
	
}

void Tile::Update(float dt)
{
}

void Tile::Draw(float dt)
{
	this->_data->window.draw(_rect);
}
