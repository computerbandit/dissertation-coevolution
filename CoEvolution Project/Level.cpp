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
				std::string texture = Tile::GetTexture(tileID);	

				if (texture != AIR_TILE_TEX) {
					sf::Sprite spriteTile;
					spriteTile.setTexture(this->_data->assetManager.GetTexture(texture));
					AssetManager::Rescale(spriteTile, sf::Vector2f(64, 64));

					sf::Vector2f pos((tokennum + totalLength)*spriteTile.getGlobalBounds().width,linenum*spriteTile.getGlobalBounds().height);
					spriteTile.setPosition(pos);
					_tilemap.push_back(Tile(tileID, spriteTile, Tile::GetIfSolid(tileID)));
					if (tileID == 101 || tileID == 102) {
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


bool Level::Collision(sf::FloatRect rect)
{
	//add some optimization or something 
	for (Tile& tile : _tilemap) {
		if (this->_data->camera.GetCameraBox().intersects(tile.GetSprite().getGlobalBounds()) && tile.IsSolid()) {
			if (rect.intersects(tile.GetSprite().getGlobalBounds())) {
				return true;
			}
		}
	}
	return false;
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
