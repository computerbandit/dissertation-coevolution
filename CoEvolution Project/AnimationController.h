#pragma once

#include <SFML/Graphics.hpp>

class AnimationController {
public:
	AnimationController(sf::Sprite& sprite);
	virtual ~AnimationController() {}



private:
	sf::Sprite& _spriteRef;
	

};
