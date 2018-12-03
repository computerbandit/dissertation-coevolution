#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:

	Tile(int tileID, sf::Sprite sprite, bool solid);
	~Tile() {}

	const int& GetTileID() const;
	const sf::Sprite& GetSprite() const;
	const bool& IsSolid() const;
	const sf::FloatRect& GetHitBox() const;
	static bool GetIfSolid(int id);

private:
	sf::Sprite _sprite;
	sf::FloatRect _box;
	int _tileID;
	bool _solid;
};