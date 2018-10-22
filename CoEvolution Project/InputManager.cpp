#include "InputManager.h"

bool InputManager::IsSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow & window)
{
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect tempRect((int)sprite.getPosition().x, (int)sprite.getPosition().y, (int)sprite.getGlobalBounds().width, (int)sprite.getGlobalBounds().height);
		if (tempRect.contains(GetMousePosition(window))) {
			return true;
		}
	}
	return false;
}

sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow & window)
{
	return sf::Mouse::getPosition(window);
}
