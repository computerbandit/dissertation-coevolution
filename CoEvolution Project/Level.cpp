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
	int linenum = 0;
	int section = 0;
	int sectionLength = 0;
	int totalLength = 0;
	bool newCheckpoint = false;
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			int tokennum = 0;
			while (std::getline(ss, token, ',')) {
				if (linenum == 0) {
					_tilesize = std::stof(token);
				}
				else {
					//if it is the start of a new section we need to add a checkpoint at the start
					if (token == "#") {
						newCheckpoint = true;
						section++;
						totalLength += sectionLength;
						sectionLength = 0;
						linenum = 0;
						break;
					}
					std::string texture = "";
					unsigned int tileID = std::stoi(token);
					sf::Color color = sf::Color::Transparent;
					switch (tileID)
					{
					case 0:
						texture = "Air";
						break;

					case 101:
						texture = "Checkpoint";
						break;

					case 999:
						texture = "Finish_Line";
						break;
					case 1:
						texture = "Tile";
						//green
						color = sf::Color(109,190,48);
						break;
					case 2:
						texture = "Tile";
						//
						color = sf::Color(172, 50, 50);
						break;
					case 3:
						texture = "Tile";
						color = sf::Color(251,242, 54);
						break;
					default:
						texture = "Air";
						break;
					}
					sf::Sprite spriteTile;
					float scale = 1.0f;
					bool solid = false;
					if (texture != "Air") {
						spriteTile.setTexture(this->_data->assetManager.GetTexture(texture));
						scale = _tilesize / spriteTile.getGlobalBounds().width;
						spriteTile.setScale(scale, scale);
						if (texture == "Tile") {
							solid = true;
							spriteTile.setColor(color);
						}
					
					}
					spriteTile.setPosition(sf::Vector2f(
						(tokennum + totalLength)*_tilesize,
						linenum*_tilesize));

					
					_tilemap.push_back(Tile(tileID, spriteTile, solid));
					if (tokennum > sectionLength) sectionLength = tokennum;
					if (tokennum == 0 && newCheckpoint) {
						if (tileID == 101 || tileID == 999) {
							//if the tile is a starting tile then spawn the player there
							_checkpoint.push_back(sf::Vector2f((
								tokennum + totalLength)*_tilesize,
								linenum*_tilesize));
							newCheckpoint = false;
						}
					}
				}
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
	case 2:
		this->LoadLevelFromTextFile(LEVEL_2);
		break;

	default:
		this->LoadLevelFromTextFile(LEVEL_1);
		break;
	}
	_currentLevel = num;
}

void Level::LoadNextLevel()
{
	std::cout << "Hello starting new Level" << std::endl;
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
