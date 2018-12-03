#include "Tile.h"
#include "DEFINITIONS.h"

Tile::Tile(int tileID, sf::Sprite sprite, bool solid):_sprite(sprite), _solid(solid), _tileID(tileID)
{
	this->_box = this->_sprite.getGlobalBounds();
}

const int & Tile::GetTileID() const
{
	return this->_tileID;
}

const sf::Sprite & Tile::GetSprite() const
{
	return this->_sprite;
}

const bool & Tile::IsSolid() const
{
	return this->_solid;
}

const sf::FloatRect & Tile::GetHitBox() const
{
	return this->_box;
}

bool Tile::GetIfSolid(int id)
{
	if (id == 0) {
		return false;
	}

	if(id < 32) {
		return true;
	}
	else {
		return false;
	}
}
