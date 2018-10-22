#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile(int tileID, sf::Sprite sprite, bool solid);
	~Tile() {}

	const sf::Sprite& GetSprite() {
		return _sprite;
	}

private:
	sf::Sprite _sprite;
	int _tileID;
	bool _solid;
};