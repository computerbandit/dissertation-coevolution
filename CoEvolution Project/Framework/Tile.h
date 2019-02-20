#pragma once
#include <SFML/Graphics.hpp>

//Tile IDS
#define AIR_TILE 0

//Grass Tiles
#define GRASS_TOP_LEFT 1
#define GRASS_TOP 2
#define GRASS_TOP_RIGHT 3
#define GRASS_MIDDLE_LEFT 4
#define GRASS_MIDDLE 5
#define GRASS_MIDDLE_RIGHT 6
#define GRASS_BOTTOM_LEFT 7
#define GRASS_BOTTOM 8
#define GRASS_BOTTOM_RIGHT 9
#define TEST_TILE 10
#define PURPLE_TILE 11
#define GREEN_TILE 12
#define BLUE_TILE 13
#define GRAY_TILE 14

#define CHECKPOINT_TILE 32
#define FINISH_LINE_TILE 33

#define DEATH_TILE 63


class Tile {
public:

	Tile(int tileID, sf::Sprite sprite, bool solid);
	~Tile() {}

	const int& getTileID() const;
	const sf::Sprite& getSprite() const;
	const bool& isSolid() const;
	const sf::FloatRect& getHitBox() const;
	static bool getIfSolid(int id);

private:
	sf::Sprite _sprite;
	sf::FloatRect _box;
	int _tileID;
	bool _solid;
	bool _interactable;
};