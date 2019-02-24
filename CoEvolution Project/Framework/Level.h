#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Tile.h"

typedef std::vector<Tile> Tilemap;
typedef std::vector<int> HMap;

class Level {
public:
	Level(GameDataRef data, std::string fileName, float time);
	Level(HMap map, GameDataRef data, std::string fileName, float time);
	Level(Level lvlA, Level lvlB);
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

	void loadLevelFromTextFile(std::string fileName);

	void createLevelFromHeightMap(HMap map);
	Tile* tileAt(int i, int j);

	GameDataRef _data;
	Tilemap _tilemap;
	std::string _fileName;
	int _width, _height;
	std::vector<sf::Vector2f> _checkpoint;
	sf::Sprite _background;
	float _timeToComplete;
};

class Noise {
public:
	//given a width and height and perameters gen a hight map with the constrains
	static HMap GenHeightMap(sf::Vector2i wh, int max, int min, int iota);
	static void ouputHeightMap(HMap& map);
	static float randomFloat(float Min, float Max);
private:
	
};


