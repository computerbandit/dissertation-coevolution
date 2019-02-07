#pragma once
#include <SFML/Graphics.hpp>

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