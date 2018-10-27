#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class AssetManager {
public:
	AssetManager() {}
	~AssetManager() {}

	void LoadTexture(std::string name, std::string fileName);
	sf::Texture &GetTexture(std::string name);
	void UnloadTexture(std::string name);

	void LoadFont(std::string name, std::string fileName);
	sf::Font &GetFont(std::string name);

	static void Rescale(sf::Sprite& sprite, sf::Vector2f newSize);
private:
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;
};
