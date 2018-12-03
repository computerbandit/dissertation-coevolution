#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "DEFINITIONS.h"

Level::Level(GameDataRef data, std::string filePath): _data(data)
{
	LoadLevelFromTextFile(filePath);
}

void Level::LoadLevelFromTextFile(std::string filePath)
{
	this->_tilemap = std::vector<Tile>();
	this->_checkpoint = std::vector<sf::Vector2f>();
	int i = 0, j = 0;
	this->_width = 0;
	this->_height = 0;
	std::ifstream file;
	std::string line;
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;

			while (std::getline(ss, token, ',')) {
				int tileID = std::stoi(token);
				sf::Sprite spriteTile;
				spriteTile.setTexture(this->_data->assetManager.GetTexturesheet(TILES).GetTexture(tileID));
				AssetManager::Rescale(spriteTile, SCALE_FACTOR);
				//change the width and height scaling
				sf::Vector2f pos(j*TILE_SIZE, i*TILE_SIZE);
				spriteTile.setPosition(pos);
				_tilemap.push_back(Tile(tileID, spriteTile, Tile::GetIfSolid(tileID)));
				if (tileID == CHECKPOINT_TILE || tileID == FINISH_LINE_TILE) {
					_checkpoint.push_back(pos);
				}
				j++;
			}
			i++;
			if(this->_width == 0){
				this->_width = j;
			}
			j = 0;
		}
	}
	this->_height = i;
	file.close();
}

Tile * Level::TileAt(int i, int j)
{
	return &_tilemap.at((j * this->_width) + i);
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
	int left_tile = (int)rect.left / TILE_SIZE;
	int right_tile = (int)(rect.left + rect.width) / TILE_SIZE;
	int top_tile = (int)rect.top / TILE_SIZE;
	int bottom_tile = (int)(rect.top + rect.height) / TILE_SIZE;
	
	if (left_tile < 0) left_tile = 0;
	if (right_tile > this->_width - 1) right_tile = this->_width - 1;
	if (top_tile < 0) top_tile = 0;
	if (bottom_tile > this->_height - 1) bottom_tile = this->_height - 1;
	
	//only check the tiles near the player maybe some how
	bool any_collision = false;
		for (int i = left_tile; i <= right_tile; i++)
		{
			for (int j = top_tile; j <= bottom_tile; j++)
			{
				if (this->TileAt(i, j)->IsSolid())
				{
					any_collision = true;
				}
			}
		}
		return any_collision;
}

const sf::Vector2f& Level::GetCheckpoint(int num) const 
{
	return _checkpoint.at(num);
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

const sf::Vector2f & Level::GetFinishFlagPosition() const
{
	return this->_checkpoint.back();
}
