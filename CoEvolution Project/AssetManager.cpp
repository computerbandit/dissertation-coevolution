#include "AssetManager.h"


void AssetManager::LoadTexture(std::string name, std::string fileName)
{
	sf::Texture texture;
	if (texture.loadFromFile(fileName)) {
		this->_textures[name] = texture;
	}
}

sf::Texture & AssetManager::GetTexture(std::string name)
{
	return this->_textures[name];
}

void AssetManager::LoadTexturesheet(std::string name, std::string fileName, sf::Vector2u texturesize)
{
	sf::Image image;
	if (image.loadFromFile(fileName)) {
		Texturesheet texturesheet(image, texturesize);
		this->_texturesheets[name] = texturesheet;
	}

}

Texturesheet & AssetManager::GetTexturesheet(std::string name)
{
	return this->_texturesheets.at(name);
}

void AssetManager::LoadFont(std::string name, std::string fileName)
{
	sf::Font font;
	if (font.loadFromFile(fileName)) {
		this->_fonts[name] = font;
	}
}

sf::Font & AssetManager::GetFont(std::string name)
{
	return this->_fonts.at(name);
}

void AssetManager::Rescale(sf::Sprite & sprite, sf::Vector2f newSize)
{
	sprite.setScale(newSize.x / sprite.getGlobalBounds().width, newSize.y / sprite.getGlobalBounds().height);
}

void AssetManager::Rescale(sf::Sprite & sprite, float factor)
{
	sprite.setScale(factor, factor);
}
