#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Tile.h"

typedef std::vector<Tile> Tilemap;

class Level {
public:
	Level(GameDataRef data, std::string filePath);
	~Level() {}

	void Draw();

	bool Collision(const sf::FloatRect &rect);
	std::vector<Tile*> GetTilesInArea(const sf::FloatRect &rect);
	bool CollisionWithTile(const sf::FloatRect &rect, int tileID);
	const sf::Vector2f& GetCheckpoint(int num) const;
	bool LastCheckpoint(int num);
	const sf::Vector2f& GetFinishFlagPosition() const;
	
private:

	void LoadLevelFromTextFile(std::string filePath);
	Tile* TileAt(int i, int j);

	GameDataRef _data;
	Tilemap _tilemap;
	int _width, _height;

	std::vector<sf::Vector2f> _checkpoint;
	sf::Sprite _background;
};
