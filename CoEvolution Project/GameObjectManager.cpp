#include "GameObjectManager.h"

GameObjectManager::~GameObjectManager()
{
	ClearEntities();
}

void GameObjectManager::AddEntity(IEntity * e)
{
	this->_entities.push_back(e);
}


void GameObjectManager::ClearEntities()
{
	this->_entities.clear();
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

