#pragma once
#include <vector>
#include <string>
#include <map>
#include "SFML/Graphics.hpp"
#include "../Neural/IFitness.h"
#include "Game.h"
#include "Tile.h"

typedef std::vector<Tile> Tilemap;
typedef std::map<ObjectLayer, std::vector<IEntity*>> Entitymap;
typedef std::vector<int> HMap;

class Level : public IFitness{
public:
	Level() {}
	Level(GameDataRef data, std::string fileName, float time);
	Level(GameDataRef data, Tilemap tilemap, float width, float height, std::string fileName, float time);
	Level(HMap map, GameDataRef data, std::string fileName, float time);
	Level(Level lvlA, Level lvlB, std::string fileName);
	~Level() {}

	void draw();
	void spawnEntities();

	bool collision(const sf::FloatRect &rect);
	std::vector<Tile*> getTilesInArea(const sf::FloatRect &rect);
	bool collisionWithTile(const sf::FloatRect &rect, int tileID);
	const sf::Vector2f& getCheckpoint(int num) const;
	bool lastCheckpoint(int num);
	const sf::Vector2f& getFinishFlagPosition() const;
	void displayTilemap();
	
	std::vector<Level> splitLevel();

	std::vector<std::vector<std::vector<std::string>>> chromosomeToSections();
	std::vector<std::string> sectionsToChromosome(std::vector<std::vector<std::vector<std::string>>> sections);

	//convert the current tilemap the a chromosome
	std::vector<std::string> levelToChromosome();
	//convert a chromosome to a tilemap for the level
	void sectionsToLevel(std::vector<std::vector<std::vector<std::string>>> sections);

	void writeTileData(std::string path, std::string token, std::string subfolder, std::string filename);

	inline std::string& getFileName() { return _fileName; }
	inline Tilemap& getTileMap() { return _tilemap; }
	inline int& getWidth() { return _width; }
	inline int& getHeight() { return _height; }
	inline std::vector<sf::Vector2f>& getCheckpoint() { return _checkpoint; }
	inline GameDataRef getGameData() { return _data; }
private:
	void loadLevelFromTextFile(std::string fileName);
	void loadEntitiesFromTextFile(std::string fileName);
	void writeTileData(std::vector<std::string> tileData);
	void writeEntityData(std::vector<std::string> entityData);
	void createLevelFromHeightMap(HMap map);
	void stichLevels(Level& lvlA, Level& lvlB);

	void pitFallLevel(std::vector<std::string>& tileData, HMap& map, float pitRate);
	void trapLevel(std::vector<std::string>& tileData, HMap& map, float trapRate);
	void platformLevel(std::vector<std::string>& tileData);

	void addCoins(std::vector<std::string>& tileData, std::vector<std::string>& entityData);

	Tile* tileAt(int i, int j);

	GameDataRef _data;
	Tilemap _tilemap;
	Entitymap _entitymap;
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
	static float randomFloat(float min, float max);
	static int randomInt(int min, int max);
	
};


