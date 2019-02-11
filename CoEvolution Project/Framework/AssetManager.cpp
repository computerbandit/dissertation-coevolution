#include "AssetManager.h"


void AssetManager::loadTexture(std::string name, std::string fileName)
{
	sf::Texture texture;
	if (texture.loadFromFile(fileName)) {
		this->_textures[name] = texture;
	}
}

sf::Texture & AssetManager::getTexture(std::string name)
{
	return this->_textures[name];
}


void AssetManager::loadTexturesheet(std::string name, std::string fileName, sf::Vector2u texturesize)
{
	sf::Image image;
	if (image.loadFromFile(fileName)) {
		Texturesheet texturesheet(image, texturesize);
		this->_texturesheets[name] = texturesheet;
	}

}

Texturesheet & AssetManager::getTexturesheet(std::string name)
{
	return this->_texturesheets.at(name);
}

void AssetManager::loadAnimationState(std::string name, Texturesheet * sheet, int startindex, int length, float time, bool single)
{
	this->_animinations[name] = AnimationState(name, sheet, startindex, length, time, single);
}

AnimationState & AssetManager::getAnimationState(std::string name)
{
	return this->_animinations.at(name);
}

void AssetManager::loadFont(std::string name, std::string fileName)
{
	sf::Font font;
	if (font.loadFromFile(fileName)) {
		this->_fonts[name] = font;
	}
}

sf::Font & AssetManager::getFont(std::string name)
{
	return this->_fonts.at(name);
}

void AssetManager::rescale(sf::Sprite & sprite, sf::Vector2f newSize)
{
	sprite.setScale(newSize.x / sprite.getGlobalBounds().width, newSize.y / sprite.getGlobalBounds().height);
}

void AssetManager::rescale(sf::Sprite & sprite, float factor)
{
	sprite.setScale(factor, factor);
}
