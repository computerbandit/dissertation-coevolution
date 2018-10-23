#include "GameObjectManager.h"

void GameObjectManager::AddEntity(IEntity * e)
{
	this->_entities.push_back(e);
}

void GameObjectManager::AddTile(Tile * t)
{
	this->_collisionmap.push_back(t);
}

void GameObjectManager::ClearEntities()
{
	this->_entities.clear();
}


void GameObjectManager::ClearCollisionMap()
{
	this->_collisionmap.clear();
}

void GameObjectManager::Draw(float dt)
{
	for (IEntity* e : _entities) {
		e->Draw(dt);
	}
}

void GameObjectManager::Update(float dt)
{
	for (IEntity* e : _entities) {
		e->Update(dt);
	}
}

