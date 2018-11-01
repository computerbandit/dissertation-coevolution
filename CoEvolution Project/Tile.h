#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile(int tileID, sf::Sprite sprite, bool solid, bool topTile);
	~Tile() {}

	int& GetTileID() {
		return _tileID;
	}

	sf::Sprite& GetSprite() {
		return _sprite;
	}

	bool IsSolid() {
		return _solid;
	}

	sf::FloatRect& GetHitBox() {
		return _box;
	}


	static bool GetIfSolid(int id);
	static bool GetIfTop(int id);
private:
	sf::Sprite _sprite;
	sf::FloatRect _box;
	int _tileID;
	bool _solid;
};