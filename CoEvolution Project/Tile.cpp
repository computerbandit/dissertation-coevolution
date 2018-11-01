#include "Tile.h"
#include "DEFINITIONS.h"

Tile::Tile(int tileID, sf::Sprite sprite, bool solid, bool topTile):_sprite(sprite), _solid(solid), _tileID(tileID)
{
	//if the tile does not have anything ontop of it then the float rect needs to be slightly smaller on the top
	this->_box = this->_sprite.getGlobalBounds();
	if (topTile) {
		this->_box.height -= (TILE_SIZE / 8);
		this->_box.top += (TILE_SIZE / 8);
	}

}

bool Tile::GetIfSolid(int id)
{
	if (id < 32) {
		return true;
	}
	else {
		return false;
	}
}

bool Tile::GetIfTop(int id)
{
	if (id >= 1 && id <= 3) {
		return true;
	}
	else {
		return false;
	}
}
