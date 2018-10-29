#include "Tile.h"
#include "TILE_DEFINITIONS.h"

Tile::Tile(int tileID, sf::Sprite sprite, bool solid):_sprite(sprite), _solid(solid), _tileID(tileID)
{
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
