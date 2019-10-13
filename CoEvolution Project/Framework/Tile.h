#pragma once
#include <SFML/Graphics.hpp>

//Tile IDS


//Grass Tiles
#define GRASS_TOP_LEFT 0
#define GRASS_TOP_CONNECTED 1
#define GRASS_TOP_RIGHT 2
#define GRASS_TOP 3
#define GRASS_MIDDLE_LEFT 4
#define GRASS_MIDDLE_CONNECTED 5
#define GRASS_MIDDLE_RIGHT 6
#define GRASS_MIDDLE 7
#define GRASS_BOTTOM_LEFT 8
#define GRASS_BOTTOM_CONNECTED 9
#define GRASS_BOTTOM_RIGHT 10
#define GRASS_BOTTOM 11
#define GRASS_MIDDLE_TSECTION 12

#define TEST_TILE 24
#define PURPLE_TILE 25
#define GREEN_TILE 26
#define BLUE_TILE 27
#define GRAY_TILE 28

#define CHECKPOINT_TILE 32
#define FINISH_LINE_TILE 33
#define SPIKE_TILE 34

#define BOTTOMOFLEVEL_TILE 60
#define AIR_TILE 61
#define DEATH_TILE 62
#define ERROR_TILE 63

class Tile {
public:

	Tile() {}
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