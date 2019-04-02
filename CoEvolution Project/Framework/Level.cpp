#include "Level.h"
#include "GameObjectManager.h"
#include "../Entity/Coin.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "DEFINITIONS.h"


Level::Level(GameDataRef data, std::string fileName, float time) : _data(data), _fileName(fileName)
{
	this->_width = 0;
	this->_height = 0;
	loadLevelFromTextFile(_fileName);
	loadEntitiesFromTextFile(_fileName);
	this->setChromosome(levelToChromosome());

}

Level::Level(GameDataRef data, Tilemap tilemap, float width, float height, std::string fileName, float time): _data(data), _fileName(fileName), _tilemap(tilemap), _width(width), _height(height)
{
	//set the checkpoints
	for (int x = 0; x < this->_width; x++) {
		for (int y = 0; y < this->_height; y++) {
			Tile& tile = _tilemap.at((y*this->_width) + x);
			if (tile.getTileID() == CHECKPOINT_TILE || tile.getTileID() == FINISH_LINE_TILE) {
				_checkpoint.push_back(sf::Vector2f(tile.getHitBox().left, tile.getHitBox().top));
			}
		}
	}
	this->setChromosome(levelToChromosome());
}

Level::Level(HMap map, GameDataRef data, std::string fileName, float time) : _data(data), _fileName(fileName)
{
	this->_width = 0;
	this->_height = 0;
	this->createLevelFromHeightMap(map);
	loadLevelFromTextFile(_fileName);
	loadEntitiesFromTextFile(_fileName);
	this->setChromosome(levelToChromosome());
}

Level::Level(Level lvlA, Level lvlB, std::string fileName): _fileName(fileName), _data(lvlA._data)
{
	this->_width = 0;
	this->_height = 0;
	//stich levels together function;
	stichLevels(lvlA, lvlB);
	loadLevelFromTextFile(_fileName);
	loadEntitiesFromTextFile(_fileName);
	this->setChromosome(levelToChromosome());
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
	std::string filePath = ((fileName == "") ? _fileName : fileName) + ".tilemap";
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
	else {
		std::cout << "ERROR: Tile map not loaded properly!" << std::endl;
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

void Level::loadEntitiesFromTextFile(std::string fileName)
{
	this->_entitymap = std::map<ObjectLayer, std::vector<IEntity*>>();
	for (int i = 0; i <= int(TOP_LAYER); i++) {
		std::vector<IEntity*> layer = std::vector<IEntity*>();
		this->_entitymap.emplace(static_cast<ObjectLayer>(i), layer);
	}
	std::ifstream file;
	std::string line;
	std::string filePath = ((fileName == "") ? _fileName : fileName) + ".entitymap";
	bool newLayer = false;
	bool addEntity = false;
	ObjectLayer layer = ObjectLayer::PLAYER_LAYER;
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ':')) {
				if (token == "L") {
					newLayer = true;
					addEntity = false;
				}
				else if (token == "E") {
					addEntity = true;
					newLayer = false;
				}
				else {
					if (newLayer) {
						if (token == "BACKGROUND_LAYER") {
							layer = ObjectLayer::BACKGROUND_LAYER;
						}
						else if (token == "LEVEL_LAYER") {
							layer = ObjectLayer::LEVEL_LAYER;
						}
						else if (token == "COIN_LAYER") {
							layer = ObjectLayer::COIN_LAYER;
						}
						else if (token == "PLAYER_LAYER") {
							layer = ObjectLayer::PLAYER_LAYER;
						}
						else if (token == "ENEMEY_LAYER") {
							layer = ObjectLayer::ENEMEY_LAYER;
						}
						else if (token == "TOP_LAYER") {
							layer = ObjectLayer::TOP_LAYER;
						}
					}
					else if (addEntity) {
						std::stringstream ss(token);
						std::string data;
						float xpos = 0.0f, ypos = 0.0f;
						int i = 0;
						std::string eType = "";
						while (std::getline(ss, data, ',')) {
							if (i == 0) {
								eType = data;
								i++;
								continue;
							}
							if (eType == "COIN") {
								if (i == 1) {
									xpos = TILE_SIZE * std::stof(data);
								}
								else if (i == 2) {
									ypos = TILE_SIZE * std::stof(data);
								}
							}
							i++;
						}
						if (eType == "COIN") {
							this->_entitymap.at(layer).push_back(new Coin(this->_data, sf::Vector2f(xpos, ypos)));
						}
					}
				}
			}	
		}
	}
	file.close();
}

void Level::writeTileData(std::vector<std::string> tileData)
{
	//parse the data to the level file
	std::string formatedTileData = "";
	int y = 0;
	for (int x = 0; x < int(tileData.size()); x++) {
		if (x%this->_width == (this->_width - 1)) {
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)));

		}
		else if ((x != 0) && (x%this->_width) == 0) {
			formatedTileData.append("\n");
			y++;
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)) + ",");
		}
		else {
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)) + ",");
		}
	}

	std::ofstream csv;
	csv.open(_fileName + ".tilemap");
	csv << formatedTileData;
	csv.close();
}

void Level::writeEntityData(std::vector<std::string> entityData)
{
	//parse the data to the level file
	std::string formatedEntityData = "";
	int y = 0;
	for (int x = 0; x < int(entityData.size()); x++) {
		formatedEntityData.append(entityData.at(x));
		if (x < int(entityData.size()) - 1) {
			formatedEntityData.append("\n");
		}
	}

	std::ofstream csv;
	csv.open(_fileName + ".entitymap");
	csv << formatedEntityData;
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
	std::vector<std::string> tileData = std::vector<std::string>(_height*_width);
	int currentHeight = this->_height;
	for (int x = 0; x < int(tileData.size()); x++) {
		//bottom layer is death tiles
		if ((x != 0) && (x%this->_width == 0)) {
			currentHeight--;
		}

		if (map.at(x%this->_width) >= currentHeight) {
			tileData.at(x) = "00";
		}
		else {
			tileData.at(x) = "61";
		}

	}

	//this could be where the section is designed, so a case random case statement that invokes a series of level gen passes eg: pitfalls, long bridge, traps, back tracks, doors...
	//add the pit falls and danger tiles
	//drop some random bad tiles on the top level
	//this could incoperate the some kind of difficulty rating

	//add pitfalls to the levelsection with 20% chance to make a pit each column.
	trapLevel(tileData, map, 0.60f);
	pitFallLevel(tileData, map, 0.50f);
	
	for (int x = (this->_height - 1)*(this->_width); x < int(tileData.size()); x++) {
		tileData.at(x) = "60";
	}

	//add checkpoints and final flag
	for (int y = 1; y < int(tileData.size()); y += this->_width) {
		//loop down each layers until you find the first solid block
		if (tileData.at(y) != "61") {
			tileData.at(y - this->_width) = "32";
			break;
		}
	}
	for (int y = this->_width - 2; y < int(tileData.size()); y += this->_width) {
		//loop down each layers until you find the first solid block
		if (tileData.at(y) != "61") {
			tileData.at(y - this->_width) = "33";
			break;
		}

	}

	writeTileData(tileData);

	//now add entities to the entity map
	std::vector<std::string> entityData = std::vector<std::string>();
	addCoins(tileData, entityData);
	
	writeEntityData(entityData);

}

void Level::stichLevels(Level & lvlA, Level & lvlB)
{
	this->_width = lvlA.getWidth() + lvlB.getWidth() - 3; //accounting for the stiching process.
	Tilemap& tilemapA = lvlA.getTileMap();
	Tilemap& tilemapB = lvlB.getTileMap();

	int yposA = 0;
	for (int y = 0; y < int(lvlA.getHeight()); y++) {
		Tile& tile = tilemapA.at(y*lvlA.getWidth() + (lvlA.getWidth() - 2));
		if (tile.getTileID() == FINISH_LINE_TILE) {
			yposA = y;
			break;
		}
	}
	int yposB = 0;
	for (int y = 0; y < int(lvlB.getHeight()); y++) {
		Tile& tile = tilemapB.at(y*lvlB.getWidth() + 1);
		if (tile.getTileID() == CHECKPOINT_TILE) {
			yposB = y;
			break;
		}
	}

	int flagYPosDelta = yposA - yposB;//if its negative then add air to that number of air rows to the top 
	bool partAShift = false;


	int newHeight = 0;
	if (flagYPosDelta > 0) {
		//shift lvlb down by that amount
		newHeight = lvlB.getHeight() + flagYPosDelta;
		this->_height = std::max(newHeight, lvlA.getHeight());
	}
	else {
		partAShift = true;
		flagYPosDelta = std::abs(flagYPosDelta);
		newHeight = lvlA.getHeight() + flagYPosDelta;
		this->_height = std::max(newHeight, lvlB.getHeight());
	}

	//given these two levels can we put them together to make a bigger one

	std::vector<std::string> tileData = std::vector<std::string>(this->_height*this->_width);

	for (std::string& td : tileData) {
		td = "61";
	}
	int index = 0;
	std::string tileString = "";
	for (int x = 0; x < lvlA.getWidth()-1; x++) {
		for (int y = 0; y < lvlA.getHeight(); y++) {
			index = y * lvlA.getWidth() + x;
			Tile& tile = tilemapA.at(index);
			tileString = std::to_string(tile.getTileID());
			tileString = (tile.getTileID() < 10) ? "0" + tileString : tileString;
			if (partAShift) {
				tileData.at((flagYPosDelta + y)*this->_width + x) = tileString;
			}
			else {
				tileData.at(y*this->_width + x) = tileString;
			}
		}
	}
	for (int x = 1; x < lvlB.getWidth(); x++) {
		for (int y = 0; y < lvlB.getHeight(); y++) {
			index = y * lvlB.getWidth() + x;
			Tile& tile = tilemapB.at(index);
			tileString = std::to_string(tile.getTileID());
			tileString = (tile.getTileID() < 10) ? "0" + tileString : tileString;
			if (partAShift) {
				tileData.at((y*this->_width) + (x + lvlA.getWidth() - 3)) = tileString;
			}
			else {
				tileData.at(((flagYPosDelta + y)* this->_width) + (x + lvlA.getWidth() - 3)) = tileString;
			}
		}
	}
	writeTileData(tileData);
}
//for a given level display the tile map in the console, used for debuging
void Level::displayTilemap()
{
	int x = 0;
	for (Tile& tile : this->_tilemap) {
		std::string	tileString = std::to_string(tile.getTileID());
		tileString = (tile.getTileID() < 10) ? "0" + tileString : tileString;
		std::cout << tileString + " ";
		if ( (x+1) % this->_width == 0 && x!=0) {
			std::cout << "\n";
		}
		x++;
	}


}

void Level::pitFallLevel(std::vector<std::string>& tileData, HMap& map, float pitRate)
{
	std::vector<bool> pitfallMarker = std::vector<bool>(this->_width);
	for (int i = 0; i < int(pitfallMarker.size()); i++) {
		pitfallMarker[i] = false;
	}

	int lastHeight = map.at(1);
	int delta = 0;
	bool clearedForPit = false;
	for (int x = 2; x < this->_width - 2; x++) {
		delta = lastHeight - map.at(x + 1);

		if (pitfallMarker.at(x - 1) && pitfallMarker.at(x - 2) && !pitfallMarker.at(x - 3) && (delta > 0)) {
			//if there was one last column then the is a much lower chance to make other
			if (Noise::randomFloat(0.0f, 1.00f) >= pitRate + 0.10f) {
				pitfallMarker.at(x) = true;
			}
		}
		else if (pitfallMarker.at(x - 1) && !pitfallMarker.at(x - 2) && (delta > -1)) {
			//if there was one last column then the is a much lower chance to make other
			if (Noise::randomFloat(0.0f, 1.00f) >= pitRate + 0.10f) {
				pitfallMarker.at(x) = true;
			}
		}
		else if (!pitfallMarker.at(x - 1) && (delta > -1)) {
			if (Noise::randomFloat(0.0f, 1.00f) >= pitRate) {
				pitfallMarker.at(x) = true;
			}
		}
		
		if (!pitfallMarker.at(x)) {
			lastHeight = map.at(x);
		}
	}

	//add the pit falls, but make sure that traps either side the pits are removed
	for (int x = 2; x < this->_width - 2; x++) {
		if (pitfallMarker.at(x)) {
			for (int y = 0; y < this->_height; y++) {
				tileData.at(y * this->_width + x) = "61";
			}
			if (!pitfallMarker.at(x - 1)) {
				for (int y = 0; y < this->_height; y++) {
					if (tileData.at(y * this->_width + (x - 1)) == "34") {
						tileData.at(y * this->_width + (x - 1)) = "00";
						map.at(x - 1) += 1;
					}
				}
			}
			if (!pitfallMarker.at(x + 1)) {
				for (int y = 0; y < this->_height; y++) {
					if (tileData.at(y * this->_width + (x + 1)) == "34") {
						tileData.at(y * this->_width + (x + 1)) = "00";
						map.at(x + 1) += 1;
					}
				}
			}
		}
		
	}
}


//replace some of the level with traps 
void Level::trapLevel(std::vector<std::string>& tileData, HMap& map, float trapRate)
{
	std::vector<bool> trapMarker = std::vector<bool>(this->_width);
	for (int i = 0; i < int(trapMarker.size()); i++) {
		trapMarker[i] = false;
	}
	int lastHeight = map.at(1);
	int delta = 0;
	bool clearedForTrap = false;
	for (int x = 2; x < this->_width - 2; x++) {
		delta = lastHeight - map.at(x + 1);
		clearedForTrap = (delta >= 0) ? true : false;
		if (clearedForTrap) {
			if (!trapMarker.at(x - 1)) {
				if (Noise::randomFloat(0.0f, 1.00f) >= trapRate) {
					trapMarker.at(x) = true;
				}
			}
		}
		if (!trapMarker.at(x)) {
			lastHeight = map.at(x);
		}
	}

	//add the traps to the level
	for (int x = 2; x < this->_width - 2; x++) {
		if (trapMarker.at(x)) {
			for (int y = 0; y < this->_height; y++) {
				//less than 32 but not 0
				if (tileData.at(y * this->_width + x) == "00") {
					tileData.at(y * this->_width + x) = "34";
					map.at(x) -= 1;
					break;
				}
			}
		}
	}
}

void Level::platformLevel(std::vector<std::string>& tileData)
{


}

void Level::addCoins(std::vector<std::string>& tileData, std::vector<std::string>& entityData)
{
	std::string layer = "L:COIN_LAYER";
	std::string eType = "E:COIN";
	std::string xpos = "0";
	std::string ypos = "0";
	std::string coin = "";

	entityData.push_back(layer);
	for (int x = 2; x < this->_width - 2; x++) {
		if (Noise::randomFloat(0.0f, 1.0f) >= 0.40f) {
			for (int y = 0; y < this->_height-1; y++) {
				if (tileData.at(y * this->_width + x) != "61") {
					xpos = std::to_string(x);
					ypos = std::to_string(y - 1);
					coin = eType + "," + xpos + "," + ypos;
					entityData.push_back(coin);
					break;
				}
			}
		}
	}
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

void Level::spawnEntities()
{
	for (int i = 0; i <= int(TOP_LAYER); i++) {
		std::vector<IEntity*> layer = this->_entitymap.at(static_cast<ObjectLayer>(i));
		int layerSize = int(layer.size());
		for (int j = 0; j < layerSize; j++)
		{
			this->_data->gameObjectManager.addEntity(layer.at(j), static_cast<ObjectLayer>(i));
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
//given this level return an array of the sub levels that make it up;
std::vector<Level> Level::splitLevel()
{
	std::vector<std::vector<std::vector<std::string>>> sections = this->chromosomeToSections();
	std::vector<Level> splitLevels = std::vector<Level>();
	std::vector<Tilemap> tilemaps = std::vector<Tilemap>();
	std::vector<int> sectionWidths = std::vector<int>();
	int w = 0;
	int addiationalColumns = 3;
	for (int i = 1; i < int(sections.size() - 1); i++) {
		//need to get the width of the the sections

		if (i == int(sections.size() - 2)) {
			addiationalColumns--;
		}

		w = int(sections.at(i).size()) + addiationalColumns;
		
		sectionWidths.push_back(w);
		tilemaps.push_back(Tilemap(this->_height * w));
	}

	for (int i = 0; i < this->_height; i++) {
		int sectionNum = 0;
		for (int s = 1; s < int(sections.size() - 1); s++) {
			std::vector<std::vector<std::string>>& columns = sections.at(s);
			for (int j = 0; j < int(columns.size()); j++) {
				//need the buffer vecotor
				int tileID = std::stoi(columns.at(j).at(i));
				sf::Sprite spriteTile;
				spriteTile.setTexture(this->_data->assetManager.getTexturesheet(TILES).getTexture(tileID));
				AssetManager::rescale(spriteTile, ZOOM_FACTOR);
				//change the width and height scaling
				sf::Vector2f pos(j*TILE_SIZE, i*TILE_SIZE);
				spriteTile.setPosition(pos);
				tilemaps.at(sectionNum).at(i*sectionWidths.at(sectionNum) + (j+1)) =  Tile(tileID, spriteTile, Tile::getIfSolid(tileID));
			}
			sectionNum++;
		}
	}

	//copy the start of the next section to the end of the last section if it is no the last section

	for (int i = 0; i < this->_height; i++) {
		int sectionNum = 0;
		for (int s = 1; s < int(sections.size() - 2); s++) {
			std::vector<std::string>& firstColumn = sections.at(s+1).at(0);
			//need the buffer vecotor
			int tileID = std::stoi(firstColumn.at(i));
			if (tileID == 32) tileID = 33;
			sf::Sprite spriteTile;
			spriteTile.setTexture(this->_data->assetManager.getTexturesheet(TILES).getTexture(tileID));
			AssetManager::rescale(spriteTile, ZOOM_FACTOR);
			sf::Vector2f pos((sectionWidths.at(sectionNum) - 2)*TILE_SIZE, i*TILE_SIZE);
			spriteTile.setPosition(pos);

			tilemaps.at(sectionNum).at(i*sectionWidths.at(sectionNum) + (sectionWidths.at(sectionNum)-2)) = Tile(tileID, spriteTile, Tile::getIfSolid(tileID));
			sectionNum++;
		}
	}

	int tileID = 60;
	sf::Sprite bufferSprite;
	bufferSprite.setTexture(this->_data->assetManager.getTexturesheet(TILES).getTexture(tileID));
	AssetManager::rescale(bufferSprite, ZOOM_FACTOR);

	for (int i = 0; i < int(tilemaps.size()); i++) {
		for (int y = 0; y < this->_height; y++) {
			//change the width and height scaling
			sf::Vector2f pos(0*TILE_SIZE, y*TILE_SIZE);
			bufferSprite.setPosition(pos);
			tilemaps.at(i).at(y*sectionWidths.at(i) + 0) = Tile(tileID, bufferSprite, Tile::getIfSolid(tileID));

			pos = sf::Vector2f((sectionWidths.at(i)-1)*TILE_SIZE, y*TILE_SIZE);
			bufferSprite.setPosition(pos);
			tilemaps.at(i).at(y*sectionWidths.at(i) + (sectionWidths.at(i) - 1)) = Tile(tileID, bufferSprite, Tile::getIfSolid(tileID));
		}
		splitLevels.push_back(Level(_data, tilemaps.at(i), sectionWidths.at(i), this->_height, "Resources/temp/level", 10.0f));
	}
	return splitLevels;
}

std::vector <std::vector<std::vector<std::string>>> Level::chromosomeToSections()
{
	std::vector <std::vector<std::vector<std::string>>> sections = std::vector <std::vector<std::vector<std::string>>>();

	this->_width = 0;
	this->_height = 0;
	//get the height of the columns
	bool sectionStart = false;
	bool columnStart = false;
	for (std::string& s : this->_chromosome) {
		if ((s == "CS" || s == "SS") && columnStart) {
			break;
		}
		else if (s == "CS") {
			columnStart = true;
		}
		else if (columnStart) {
			this->_height++;
		}
	}

	int numOfSections = 0;
	for (int i = 0; i < int(this->_chromosome.size()); i++) {
		std::string& s = this->_chromosome.at(i);
		if (s == "SS") {
			numOfSections++;
			sections.push_back(std::vector<std::vector<std::string>>());
			continue;
		}
		else if (s == "CS") {
			this->_width++;
			sections.back().push_back(std::vector<std::string>());
			continue;
		}
		else {
			sections.back().back().push_back(s);
		}
	}
	return sections;
}

std::vector<std::string> Level::sectionsToChromosome(std::vector<std::vector<std::vector<std::string>>> sections)
{
	this->_chromosome.clear();
	this->_chromosome.push_back("SS");
	//convert the columns back to a chromosome
	for (std::vector<std::vector<std::string>>& columns : sections) {
		for (std::vector<std::string>& column : columns) {
			for (std::string s : column) {
				if (s == "32") {
					this->_chromosome.push_back("SS");
					break;
				}
			}
			bool sectionEnd = false;
			this->_chromosome.push_back("CS");
			for (std::string& s : column) {
				this->_chromosome.push_back(s);
				if (s == "33") {
					sectionEnd = true;
				}
			}
			if (sectionEnd) {
				this->_chromosome.push_back("SS");
			}
		}
	}

	return this->_chromosome;
}

std::vector<std::string> Level::levelToChromosome()
{
	this->_chromosome.clear();
	this->_chromosome.push_back("SS");
	for (int x = 0; x < this->_width; x++) {
		for (int y = 0; y < this->_height; y++) {
			Tile& tile = this->_tilemap.at(y*this->_width + x);
			if (tile.getTileID() == 32) {
				this->_chromosome.push_back("SS");
				break;
			}
		}
		this->_chromosome.push_back("CS");
		bool sectionEnd = false;
		for (int y = 0; y < this->_height; y++) {
			Tile& tile = this->_tilemap.at(y*this->_width + x);
			std::string tileString = std::to_string(tile.getTileID());;
			tileString = (tile.getTileID() < 10) ? "0" + tileString : tileString;
			this->_chromosome.push_back(tileString);
			if (tileString == "33") {
				sectionEnd = true;
			}
		}
		if (sectionEnd) {
			this->_chromosome.push_back("SS");
		}
	}
	return this->_chromosome;
}


void Level::sectionsToLevel(std::vector<std::vector<std::vector<std::string>>> sections)
{

	this->_height = sections.back().back().size();
	int newWidth = 0;
	for (int i = 0; i < int(sections.size()); i++) {
		newWidth += sections.at(i).size();
	}
	this->_width = newWidth;

	this->_tilemap.clear();
	for (int i = 0; i < this->_height; i++) {
		int pw = 0;
		for (std::vector<std::vector<std::string>> columns : sections) {
			for (int j = 0; j < int(columns.size()); j++) {
				int tileID = std::stoi(columns.at(j).at(i));
				sf::Sprite spriteTile;
				spriteTile.setTexture(this->_data->assetManager.getTexturesheet(TILES).getTexture(tileID));
				AssetManager::rescale(spriteTile, ZOOM_FACTOR);
				//change the width and height scaling
				sf::Vector2f pos((pw)*TILE_SIZE, i*TILE_SIZE);
				spriteTile.setPosition(pos);
				this->_tilemap.push_back(Tile(tileID, spriteTile, Tile::getIfSolid(tileID)));
				pw++;
			}
		}
	}
	_checkpoint.clear();
	for (int x = 0; x < this->_width; x++) {
		for (int y = 0; y < this->_height; y++) {
			Tile& tile = _tilemap.at((y*this->_width) + x);
			if (tile.getTileID() == CHECKPOINT_TILE || tile.getTileID() == FINISH_LINE_TILE) {
				_checkpoint.push_back(sf::Vector2f(tile.getHitBox().left, tile.getHitBox().top));
			}
		}
	}

	this->sectionsToChromosome(sections);
}

void Level::writeTileData(std::string path, std::string token, std::string subfolder, std::string filename)
{
	std::vector<std::string> tileData = std::vector<std::string>(this->_width*this->_height);
	for (int i = 0; i < int(_tilemap.size()); i++) {
		Tile& tile = _tilemap.at(i);
		std::string tileString = std::to_string(tile.getTileID());
		tileData.at(i) = (tile.getTileID() < 10) ? "0" + tileString : tileString;
	}

	//parse the data to the level file
	std::string formatedTileData = "";
	int y = 0;
	for (int x = 0; x < int(_tilemap.size()); x++) {
		if (x%this->_width == (this->_width - 1)) {
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)));

		}
		else if ((x != 0) && (x%this->_width) == 0) {
			formatedTileData.append("\n");
			y++;
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)) + ",");
		}
		else {
			formatedTileData.append(tileData.at((y*this->_width) + (x%this->_width)) + ",");
		}
	}

	if (subfolder != "") {
		subfolder = "/" + subfolder;
	}
	std::ofstream file;
	file.open("Resources/" + path + token + subfolder + "/" + filename + ".tilemap");
	file << formatedTileData;
	file.close();
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
	if (max - min <= 0 || wh.x < 5 || wh.y < 4) {
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

int Noise::randomInt(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}
