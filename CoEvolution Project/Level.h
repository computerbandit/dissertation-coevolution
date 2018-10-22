#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Tile.h"

class Level {
public:
	Level(GameDataRef data);
	~Level() {}
	bool LoadLevelFromTextFile(std::string filePath);
	void Init();
	void Cleanup();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;
	std::vector<Tile> _tilemap;
	sf::Vector2i _mapsize;
	float _tilesize;
};
