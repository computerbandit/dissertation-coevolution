#include "Tile.h"
#include "DEFINITIONS.h"

Tile::Tile(int tileID, sf::Sprite sprite, bool solid):_sprite(sprite), _solid(solid), _tileID(tileID)
{
	this->_interactable = false;
	this->_box = this->_sprite.getGlobalBounds();
}

const int & Tile::getTileID() const
{
	return this->_tileID;
}

const sf::Sprite & Tile::getSprite() const
{
	return this->_sprite;
}

const bool & Tile::isSolid() const
{
	return this->_solid;
}

const sf::FloatRect & Tile::getHitBox() const
{
	return this->_box;
}

bool Tile::getIfSolid(int id)
{

	if(id < 32) {
		return true;
	}
	else {
		return false;
	}
}
