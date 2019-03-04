#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateMachine.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "GameObjectManager.h"
#include "Camera.h"

struct GameData {
	StateMachine stateMachine;
	sf::RenderWindow window;
	Camera camera;
	AssetManager assetManager;
	InputManager inputManager;
	GameObjectManager gameObjectManager;
	sf::Vector2f gravity = sf::Vector2f(0.0f, 1000.0f);
	bool releaseAccumulator = false;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game {
public:
	Game(int width, int height, std::string title);
	void run();
private:
	const float dt = 1.0f / 60.0f;
	sf::Clock _clock;
	GameDataRef _data = std::make_shared<GameData>();
};