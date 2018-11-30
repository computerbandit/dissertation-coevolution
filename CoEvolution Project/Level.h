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

	void LoadLevel(int num);
	void LoadNextLevel();
	void Draw();

	Tile* Collision(sf::FloatRect rect);
	const sf::Vector2f& GetCheckpoint(int num) const;
	bool LastCheckpoint(int num);
	const sf::Vector2f& GetFinishFlagPosition() const;

private:
	void Cleanup();
	bool LoadLevelFromTextFile(std::string filePath);

	int _currentLevel;
	GameDataRef _data;
	Tilemap _tilemap;
	float _tilesize;
	std::vector<sf::Vector2f> _checkpoint;
	sf::Sprite _background;
};
