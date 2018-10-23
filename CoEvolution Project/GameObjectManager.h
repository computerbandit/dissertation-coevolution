#pragma once
#include <vector>
#include "IEntity.h"
#include "Tile.h"

class GameObjectManager {
public:
	GameObjectManager() {}
	~GameObjectManager() {}

	void AddEntity(IEntity* e);
	void AddTile(Tile* t);
	
	std::vector<Tile*> CollisionMap() {
		return _collisionmap;
	}
	std::vector<IEntity*> Entities() {
		return _entities;
	}

	void ClearEntities();
	void ClearCollisionMap();

	void Draw(float dt);
	void Update(float dt);

	std::vector<IEntity*> _entities;
	std::vector<Tile*> _collisionmap;
};
