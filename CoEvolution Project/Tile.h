#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile(int tileID, sf::Sprite sprite, bool solid);
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
	static bool GetIfSolid(int id);
private:
	sf::Sprite _sprite;
	int _tileID;
	bool _solid;
};