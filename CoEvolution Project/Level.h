#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Tile.h"


class Level {
public:
	Level(GameDataRef data);
	~Level() {}

	bool LoadLevelFromTextFile(std::string filePath);
	void Init();
	void Cleanup();
	void Draw();

	const float& TileSize() {
		return _tilesize;
	}

	const sf::Vector2i& MapSize() {
		return _mapsize;
	}

private:
	GameDataRef _data;
	std::vector<Tile> _tilemap;
	sf::Vector2i _mapsize;
	float _tilesize;
};
