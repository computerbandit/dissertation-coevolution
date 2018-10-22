#pragma once
#include <vector>
#include "IEntity.h"

class EntityManager {
public:
	EntityManager() {}
	~EntityManager() {}

	void Add(IEntity* e);
	void Clear();

	void Draw(float dt);
	void Update(float dt);

private:
	std::vector<IEntity*> _entities;
};
