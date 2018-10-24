#pragma once
#include <vector>
#include "IEntity.h"
#include "Tile.h"

class GameObjectManager {
public:
	GameObjectManager() {}
	~GameObjectManager();

	void AddEntity(IEntity* e);
	void ClearEntities();


	void Draw(float dt);
	void Update(float dt);

private:
	std::vector<IEntity*> _entities;
};
