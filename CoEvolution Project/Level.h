#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Tile.h"

typedef std::vector<Tile> Tilemap;

class Level {
public:
	Level(GameDataRef data);
	~Level() {}

	bool LoadLevelFromTextFile(std::string filePath);
	void Init();
	void Cleanup();
	void Draw();

	bool Collision(sf::FloatRect rect);
private:
	GameDataRef _data;
	Tilemap _tilemap;
	sf::Vector2i _mapsize;
	float _tilesize;

	sf::Sprite _background;
};
