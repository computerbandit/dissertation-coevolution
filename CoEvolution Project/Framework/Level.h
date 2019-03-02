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
	Level(Level lvlA, Level lvlB, std::string fileName);
	~Level() {}

	void draw();

	bool collision(const sf::FloatRect &rect);
	std::vector<Tile*> getTilesInArea(const sf::FloatRect &rect);
	bool collisionWithTile(const sf::FloatRect &rect, int tileID);
	const sf::Vector2f& getCheckpoint(int num) const;
	bool lastCheckpoint(int num);
	const sf::Vector2f& getFinishFlagPosition() const;

	inline std::string& getFileName() { return _fileName; }
	inline Tilemap& getTileMap() { return _tilemap; }
	inline int& getWidth() { return _width; }
	inline int& getHeight() { return _height; }
	inline std::vector<sf::Vector2f>& getCheckpoint() { return _checkpoint; }

private:

	void loadLevelFromTextFile(std::string fileName);
	void writeLevelData(std::vector<std::string> levelData);
	void createLevelFromHeightMap(HMap map);
	void stichLevels(Level& lvlA, Level& lvlB);
	Tile* tileAt(int i, int j);

	GameDataRef _data;
	Tilemap _tilemap;
	std::string _fileName;
	int _width, _height;
	std::vector<sf::Vector2f> _checkpoint;
	sf::Sprite _background;
};

class Noise {
public:
	//given a width and height and perameters gen a hight map with the constrains
	static HMap GenHeightMap(sf::Vector2i wh, int max, int min, int iota);
	static void ouputHeightMap(HMap& map);
	static float randomFloat(float Min, float Max);
private:
	
};


