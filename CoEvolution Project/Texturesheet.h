#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Texturesheet {
public:
	Texturesheet() {}
	Texturesheet(sf::Image texturesheet, sf::Vector2u spritesize);
	~Texturesheet() {}
	sf::Texture &getTexture(int index);

private:
	sf::Vector2u _texturesize;
	sf::Vector2u _sheetsize;
	std::vector<sf::Texture> _textures;

};