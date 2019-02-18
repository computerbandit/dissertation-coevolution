#pragma once
#include <vector>
#include "IEntity.h"
#include "Tile.h"

class GameObjectManager {
public:
	GameObjectManager() {}
	~GameObjectManager();

	void addEntity(IEntity* e);
	void removeEntity(IEntity* e);
	void clearEntities();

	void draw(float dt);
	void update(float dt);

private:
	std::vector<IEntity*> _entities;
};
