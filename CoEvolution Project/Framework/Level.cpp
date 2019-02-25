#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "DEFINITIONS.h"


Level::Level(GameDataRef data, std::string fileName, float time) : _data(data), _timeToComplete(time), _fileName(fileName)
{
	this->_width = 0;
	this->_height = 0;
	loadLevelFromTextFile(_fileName);
}

Level::Level(HMap map, GameDataRef data, std::string fileName, float time) : _data(data), _timeToComplete(time), _fileName(fileName)
{
	this->_width = 0;
	this->_height = 0;
	this->createLevelFromHeightMap(map);
	loadLevelFromTextFile(_fileName);
}

Level::Level(Level lvlA, Level lvlB, std::string fileName): _fileName(fileName)
{
	this->_width = 0;
	this->_height = 0;
	//stich levels together function;
	stichLevels(lvlA, lvlB);
	loadLevelFromTextFile(_fileName);
}

void Level::loadLevelFromTextFile(std::string fileName = "")
{
	this->_tilemap = std::vector<Tile>();
	this->_checkpoint = std::vector<sf::Vector2f>();
	int i = 0, j = 0;
	this->_width = 0;
	this->_height = 0;
	std::ifstream file;
	std::string line;
	std::string filePath = LEVEL_PATH + ((fileName == "") ? _fileName : fileName) + ".level";
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;

			while (std::getline(ss, token, ',')) {
				int tileID = std::stoi(token);
				sf::Sprite spriteTile;
				spriteTile.setTexture(this->_data->assetManager.getTexturesheet(TILES).getTexture(tileID));
				AssetManager::rescale(spriteTile, ZOOM_FACTOR);
				//change the width and height scaling
				sf::Vector2f pos(j*TILE_SIZE, i*TILE_SIZE);
				spriteTile.setPosition(pos);
				_tilemap.push_back(Tile(tileID, spriteTile, Tile::getIfSolid(tileID)));
				j++;
			}
			i++;
			if (this->_width == 0) {
				this->_width = j;
			}
			j = 0;
		}
	}
	this->_height = i;
	for (int x = 0; x < this->_width; x++) {
		for (int y = 0; y < this->_height; y++) {
			Tile& tile = _tilemap.at((y*this->_width) + x);
			if (tile.getTileID() == CHECKPOINT_TILE || tile.getTileID() == FINISH_LINE_TILE) {
				_checkpoint.push_back(sf::Vector2f(tile.getHitBox().left, tile.getHitBox().top));
			}
		}
	}
	file.close();
}

void Level::writeLevelData(std::vector<std::string> levelData)
{
	//parse the data to the level file
	std::string formatedLvlData = "";
	int y = 0;
	for (int x = 0; x < int(levelData.size()); x++) {
		if (x%this->_width == (this->_width - 1)) {
			formatedLvlData.append(levelData.at((y*this->_width) + (x%this->_width)));

		}
		else if ((x != 0) && (x%this->_width) == 0) {
			formatedLvlData.append("\n");
			y++;
			formatedLvlData.append(levelData.at((y*this->_width) + (x%this->_width)) + ",");
		}
		else {
			formatedLvlData.append(levelData.at((y*this->_width) + (x%this->_width)) + ",");
		}
	}

	std::ofstream csv;
	csv.open(LEVEL_PATH + _fileName + ".level");
	csv << formatedLvlData;
	csv.close();
}


void Level::createLevelFromHeightMap(HMap map)
{

	this->_width = map.size();
	//get the max height and add one for air
	int heighest = 1;
	for (int x = 0; x < this->_width; x++) {
		if (map.at(x) > heighest) {
			heighest = map.at(x);
		}
	}
	//top level is all air - just how I have decied to do it
	this->_height = heighest + 2;
	std::vector<std::string> levelData = std::vector<std::string>(_height*_width);
	int currentHeight = this->_height;
	for (int x = 0; x < int(levelData.size()); x++) {
		//bottom layer is death tiles
		if ((x != 0) && (x%this->_width == 0)) {
			currentHeight--;
		}

		if (map.at(x%this->_width) >= currentHeight) {
			int random = int(std::floor(Noise::randomFloat(11.0f, 15.0f)));
			switch (random)
			{
			case 11:
				levelData.at(x) = "11";
				break;
			case 12:
				levelData.at(x) = "12";
				break;
			case 13:
				levelData.at(x) = "13";
				break;
			case 14:
				levelData.at(x) = "14";
				break;
			default:
				levelData.at(x) = "14";
				break;
			}

		}
		else {
			levelData.at(x) = "00";
		}

	}

	for (int x = (this->_height - 1)*(this->_width); x < int(levelData.size()); x++) {
		levelData.at(x) = "63";
	}

	//add checkpoints and final flag
	for (int y = 1; y < int(levelData.size()); y += this->_width) {
		//loop down each layers until you find the first solid block
		if (levelData.at(y) != "00") {
			levelData.at(y - this->_width) = "32";
			break;
		}
	}
	for (int y = this->_width - 2; y < int(levelData.size()); y += this->_width) {
		//loop down each layers until you find the first solid block
		if (levelData.at(y) != "00") {
			levelData.at(y - this->_width) = "33";
			break;
		}
	}

	writeLevelData(levelData);

}

void Level::stichLevels(Level & lvlA, Level & lvlB)
{
	this->_height = std::max(lvlA.getHeight(), lvlB.getHeight());
	this->_width = lvlA.getWidth() + lvlB.getWidth() - 3; //accounting for the stiching process.
	Tilemap& tilemapA = lvlA.getTileMap();
	Tilemap& tilemapB = lvlB.getTileMap();

	int yposA = 0;
	for (int y = lvlA.getWidth() - 2; y < lvlA.getHeight()*lvlA.getWidth(); y += lvlA.getWidth()) {
		Tile& tile = tilemapA.at(y);
		if (tile.getTileID() == FINISH_LINE_TILE) {
			yposA = (y + 2) / lvlA.getWidth();
			break;
		}
	}
	int yposB = 0;
	for (int y = 1; y < lvlB.getHeight()*lvlB.getWidth(); y += lvlB.getWidth()) {
		Tile& tile = tilemapB.at(y);
		if (tile.getTileID() == CHECKPOINT_TILE) {
			yposB = (y + lvlB.getWidth() - 1) / lvlB.getWidth();
			break;
		}
	}

	int flagYPosDelta = yposA - yposB;//if its negative then add air to that number of air rows to the top 
	bool partA = (flagYPosDelta > 0) ? true : false;

	//given these two levels can we put them together to make a bigger one

	std::vector<std::string> levelData = std::vector<std::string>(this->_height*this->_width);
	for (std::string& td : levelData) {
		td = "00";
	}
	int index = 0;
	Tile& tile = tilemapA.at(0);
	std::string tileData = "00";
	for (int x = 0; x < this->_width; x++) {
		for (int y = 0; y < this->_height; y++) {
			
			if (x <= lvlA.getWidth() - 2 && y < lvlA.getHeight()) {
				index = y * lvlA.getWidth() + (x% this->getWidth());
				tile = tilemapA.at(index);
				tileData = std::to_string(tile.getTileID());
				tileData = (tile.getTileID() < 10) ? "0" + tileData : tileData;
				if (partA) {
					levelData.at((flagYPosDelta + y)*lvlA.getWidth() + x) = tileData;
				}
				else {
					levelData.at((y)*lvlA.getWidth() + x) = tileData;
				}
			}
			else if(x > lvlA.getWidth() - 2 && y < lvlB.getHeight()){
				index = y * lvlB.getWidth() + ((x - lvlA.getWidth() + 2) % (lvlB.getWidth()));
				tile = tilemapB.at(index);
				tileData = std::to_string(tile.getTileID());
				tileData = (tile.getTileID() < 10) ? "0" + tileData : tileData;
				if (partA) {
					levelData.at((y)*lvlB.getWidth() + x) = tileData;
				}
				else {
					levelData.at((-flagYPosDelta + y)*lvlB.getWidth() + x) = tileData;
				}
			}
		}
	}
	writeLevelData(levelData);
}

Tile * Level::tileAt(int i, int j)
{
	if (i < 0) i = 0;
	if (i > this->_width - 1) i = this->_width - 1;
	if (j < 0) j = 0;
	if (j > this->_height - 1) j = this->_height - 1;
	return &_tilemap.at((j * this->_width) + i);
}

void Level::draw()
{
	for (Tile& tile : _tilemap) {
		if (this->_data->camera.getCameraBox().intersects(tile.getSprite().getGlobalBounds())) {
			this->_data->window.draw(tile.getSprite());
		}
	}
}


bool Level::collision(const sf::FloatRect &rect)
{
	bool any_collision = false;
	for (Tile* t : this->getTilesInArea(rect)) {
		if (t->isSolid()) {
			any_collision = true;
			break;
		}
	}
	return any_collision;
}

std::vector<Tile*> Level::getTilesInArea(const sf::FloatRect &rect)
{
	int left_tile = int(rect.left / TILE_SIZE);
	int right_tile = int((rect.left + rect.width) / TILE_SIZE);
	int top_tile = int(rect.top / TILE_SIZE);
	int bottom_tile = int((rect.top + rect.height) / TILE_SIZE);

	std::vector<Tile*> tilesInArea = std::vector<Tile*>();

	for (int i = left_tile; i <= right_tile; i++)
	{
		for (int j = top_tile; j <= bottom_tile; j++)
		{
			tilesInArea.push_back(this->tileAt(i, j));
		}
	}
	return tilesInArea;

}

bool Level::collisionWithTile(const sf::FloatRect &rect, int tileID)
{
	for (Tile* t : this->getTilesInArea(rect)) {
		if (t->getTileID() == tileID) {
			return true;
		}
	}
	return false;
}

const sf::Vector2f& Level::getCheckpoint(int num) const
{
	return _checkpoint.at(num);
}

bool Level::lastCheckpoint(int num)
{
	if (num == _checkpoint.size() - 1) {
		return true;
	}
	else {
		return false;
	}
}

const sf::Vector2f & Level::getFinishFlagPosition() const
{
	return this->_checkpoint.back();
}

const float & Level::getLevelTime() const
{
	return this->_timeToComplete;
}


//Noise functions
HMap Noise::GenHeightMap(sf::Vector2i wh, int max, int min, int iota)
{
	//make sure the max and min are in range of the Widt and Height given
	//if not clamp the values to -1/+1 the range
	if (max >= wh.y) {
		max = wh.y - 1;
	}
	if (min < 1) {
		min = 2;
	}
	//needs to accomodate for level layout: air, start flag, whatever, finish flag, air;
	if (max - min <= 0 || wh.x < 5) {
		//this is an invalid map size try again
		std::cout << "ERROR: Invalid Heightmap size or perams" << std::endl;
		return HMap();
	}

	HMap map = HMap();
	map.push_back(0);
	//height is denoted by the magnitude of the int value in the vector
	int xinit = (max + min) / 2.0f;
	int delta = 0;
	for (int x = 1; x < wh.x - 1; x++) {
		//for each x value push a height value on the map vector
		//TODO: add in the Iota value for the rate of change
		float random = Noise::randomFloat(0.0f, 1.0f);
		if (random <= 0.33f) {
			delta = 0;
		}
		else if (random <= 0.66f) {
			delta = iota;
		}
		else if (random <= 1.00f) {
			delta = -iota;
		}
		xinit += delta;
		//clamp value
		if (xinit > max || xinit < min) {
			xinit = std::max(min, std::min(xinit, max));
		}
		map.push_back(xinit);
	}
	map.push_back(0);
	return map;
}

void Noise::ouputHeightMap(HMap& map)
{
	for (int x = 0; x < int(map.size()); x++) {
		for (int y = 0; y < map.at(x); y++) {
			std::cout << "[]";
		}
		std::cout << "\n";
	}
}

float Noise::randomFloat(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
