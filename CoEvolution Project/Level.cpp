#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "DEFINITIONS.h"

Level::Level(GameDataRef data): _data(data)
{
	this->Cleanup();
}

bool Level::LoadLevelFromTextFile(std::string filePath)
{
	this->Cleanup();
	std::ifstream file;
	std::string line;
	int linenum = 0, sectionLength = 0, totalLength = 0;
	bool newCheckpoint = false;
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			int tokennum = 1;

			while (std::getline(ss, token, ',')) {
				if (token == "#") {
					totalLength += sectionLength;
					sectionLength = 0;
					linenum = 0;
					tokennum++;
					break;
				}

				unsigned int tileID = std::stoi(token);

				if (tileID != AIR_TILE) {
					sf::Sprite spriteTile;
					std::string sheetname = "";
					bool topTile = Tile::GetIfTop(tileID);
					if (tileID < 100) {
						sheetname = TILES;
					}
					else if (tileID >= 100) {
						sheetname = PLATFORMS;
						tileID -= 100;
					}
					spriteTile.setTexture(this->_data->assetManager.GetTexturesheet(sheetname).GetTexture(tileID));
					AssetManager::Rescale(spriteTile, SCALE_FACTOR);
					

					//change the width and height scaling
					sf::Vector2f pos(
						(tokennum + totalLength)*TILE_SIZE,linenum*TILE_SIZE);
					spriteTile.setPosition(pos);
					_tilemap.push_back(Tile(tileID, spriteTile, Tile::GetIfSolid(tileID), topTile));
					if (tileID == CHECKPOINT_TILE || tileID == FINISH_LINE_TILE) {
						_checkpoint.push_back(pos);
					}
				}
				sectionLength = tokennum;
				tokennum++;
			}
			linenum++;
		}
		file.close();
		return true;
	}
	else {
		return false;
	}
}

void Level::LoadLevel(int num)
{
	switch (num)
	{

	case 1:
		this->LoadLevelFromTextFile(LEVEL_1);
		
		break;

	default:
		this->LoadLevelFromTextFile(LEVEL_1);
		break;
	}
	_currentLevel = num;
}

void Level::LoadNextLevel()
{
	std::cout << "starting next Level" << std::endl;
 	this->LoadLevel(_currentLevel + 1);
}

void Level::Cleanup()
{
	this->_tilemap = std::vector<Tile>();
	this->_checkpoint = std::vector<sf::Vector2f>();
	this->_tilesize = 16;
}

void Level::Draw()
{
	for (Tile& tile : _tilemap) {
		if (this->_data->camera.GetCameraBox().intersects(tile.GetSprite().getGlobalBounds())) {
			this->_data->window.draw(tile.GetSprite());
		}
	}
}


Tile* Level::Collision(sf::FloatRect rect)
{
	//add some optimization or something
	//only check the tiles near the player maybe some how
	for (Tile& tile : _tilemap) {
		if (tile.IsSolid() && this->_data->camera.GetCameraBox().intersects(tile.GetSprite().getGlobalBounds())) {
			if (rect.intersects(tile.GetHitBox())) {
				return &tile;
			}
		}
	}
	return nullptr;
}

const sf::Vector2f* Level::GetCheckpoint(int num)
{
	if (num < (int)_checkpoint.size()) {
		return &_checkpoint[num];
	}
	else {
		return nullptr;
	}
}

bool Level::LastCheckpoint(int num)
{
	if (num == _checkpoint.size() - 1) {
		return true;
	}
	else {
		return false;
	}
}
