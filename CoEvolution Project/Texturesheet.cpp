#include "Texturesheet.h"

Texturesheet::Texturesheet(sf::Image texturesheet, sf::Vector2u texturesize) : _texturesize(texturesize) 
{
	this->_sheetsize = sf::Vector2u(texturesheet.getSize().x / _texturesize.x, texturesheet.getSize().y / _texturesize.y);
	for (int y = 0; y < (int)_sheetsize.y;y++) {
		for (int x = 0; x < (int)_sheetsize.x; x++) {
			this->_textures.push_back(sf::Texture());
			this->_textures.back().loadFromImage(texturesheet, sf::IntRect(x*_texturesize.x, y*_texturesize.y, _texturesize.x, _texturesize.y));
		}
	}
}

sf::Texture & Texturesheet::GetTexture(int index)
{
	if (index < (_texturesize.x * _texturesize.y)) {
		return _textures[index];
	}
	else {
		return _textures[_textures.size()-1];
	}
}
