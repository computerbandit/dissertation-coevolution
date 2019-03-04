#pragma once
#include <SFML/Graphics.hpp>

struct Button {
	sf::Sprite _sprite;
	sf::Text _text;
	Button() {};
	Button(sf::Sprite sprite, sf::Text text) : _sprite(sprite), _text(text) {}
};