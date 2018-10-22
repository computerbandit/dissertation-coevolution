#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Level::Level(GameDataRef data): _data(data)
{
	this->_tilesize = 16;
	this->_mapsize = sf::Vector2i(0, 0);
}

bool Level::LoadLevelFromTextFile(std::string filePath)
{
	this->Cleanup();
	this->_mapsize = sf::Vector2i(0, 0);
	std::ifstream file;
	std::string line;
	int linenum = 0;
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			int tokennum = 0;
			while (std::getline(ss, token, ',')) {
				if (linenum == 0) {
					switch (tokennum)
					{
					case 0:
						_tilesize = std::stof(token);
						break;
					case 1:
						_mapsize.x = std::stoi(token);
						break;
					case 2:
						_mapsize.y = std::stoi(token);
						break;
					default:
						break;
					}
				}
				else {
					_tilemap.push_back(Tile(_data, std::stoi(token), _tilesize, _mapsize, _tilemap.size()));
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

void Level::Init()
{
	//in this we need to allocate all the textures to the correct tile and
	//then assign then a position based on the index in the map.
}

void Level::Cleanup()
{
}

void Level::Update(float dt)
{
	for (Tile tile : _tilemap) {
		if (!tile._static) {
			tile.Update(dt);
		}
	}
}

void Level::Draw(float dt)
{
	for (Tile tile : _tilemap) {
		tile.Draw(dt);
	}
}
