#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Level::Level(GameDataRef data) : _data(data)
{
	this->Cleanup();
}

bool Level::LoadLevelFromTextFile(std::string filePath)
{
	this->Cleanup();
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
						std::cout << _tilesize;
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
					std::string texture = "";
					unsigned int tileID = std::stoi(token);
					switch (tileID)
					{
					case 0:
						texture = "Air";
						break;
					case 1:
						texture = "Grass_Tile";
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
						solid = true;
					}
					spriteTile.setPosition(sf::Vector2f(tokennum*_tilesize, (linenum - 1)*_tilesize));

					_tilemap.push_back(Tile(tileID, spriteTile, solid));
					if (_tilemap.back().IsSolid()) {
						this->_data->gameObjectManager.AddTile(&(_tilemap.back()));
					}
				}
				tokennum++;
			}
			linenum++;
		}
		file.close();
		this->Init();
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
	this->_tilemap.clear();
	this->_data->gameObjectManager.ClearCollisionMap();
	this->_mapsize = sf::Vector2i(0, 0);
	this->_tilesize = 16;
}

void Level::Draw()
{
	for (Tile tile : _tilemap) {
		this->_data->window.draw(tile.GetSprite());
	}
}
