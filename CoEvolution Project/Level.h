#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Tile.h"

typedef std::vector<Tile> Tilemap;

class Level {
public:
	Level(GameDataRef data, std::string filePath, float time);
	~Level() {}

	void draw();

	bool collision(const sf::FloatRect &rect);
	std::vector<Tile*> getTilesInArea(const sf::FloatRect &rect);
	bool collisionWithTile(const sf::FloatRect &rect, int tileID);
	const sf::Vector2f& getCheckpoint(int num) const;
	bool lastCheckpoint(int num);
	const sf::Vector2f& getFinishFlagPosition() const;

	const float& getLevelTime() const;
	
private:

	void loadLevelFromTextFile(std::string filePath);
	Tile* tileAt(int i, int j);


	GameDataRef _data;
	Tilemap _tilemap;
	int _width, _height;
	std::vector<sf::Vector2f> _checkpoint;
	sf::Sprite _background;
	float _timeToComplete;
};
