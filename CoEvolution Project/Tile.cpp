#include "Tile.h"
#include "TILE_DEFINITIONS.h"

Tile::Tile(int tileID, sf::Sprite sprite, bool solid):_sprite(sprite), _solid(solid), _tileID(tileID)
{
}

std::string Tile::GetTexture(int id)
{
	switch (id)
	{
	case GRASS_TILE:
		return GRASS_TILE_TEX;
		break;
	case CHECKPOINT_TILE:
		return CHECKPOINT_TILE_TEX;
		break;
	case FINISH_LINE_TILE:
		return FINISH_LINE_TILE_TEX;
		break;
	default:
		return AIR_TILE_TEX;
		break;
	}

}

bool Tile::GetIfSolid(int id)
{
	if (id < 100) {
		return true;
	}
	else {
		return false;
	}
}
