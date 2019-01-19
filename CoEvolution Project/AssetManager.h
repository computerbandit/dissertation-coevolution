#pragma once
#include <map>
#include "Texturesheet.h"
#include <SFML/Graphics.hpp>

class AssetManager {
public:
	AssetManager() {}
	~AssetManager() {}

	void loadTexture(std::string name, std::string fileName);
	sf::Texture &getTexture(std::string name);

	//loads a image into a the manager given a name, filepath and the size of the textures
	void loadTexturesheet(std::string name, std::string fileName, sf::Vector2u texturesize);
	Texturesheet &getTexturesheet(std::string name);

	void loadFont(std::string name, std::string fileName);
	sf::Font &getFont(std::string name);

	static void rescale(sf::Sprite& sprite, sf::Vector2f newSize);
	static void rescale(sf::Sprite& sprite, float factor);
private:
	std::map<std::string, Texturesheet> _texturesheets;
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;
};
