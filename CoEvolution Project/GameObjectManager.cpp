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
		if (e->IsActive()) {
			e->Draw(dt);
		}
	}
}

void GameObjectManager::Update(float dt)
{
	for(int i = 0; i < (int)_entities.size(); i++){
		if (_entities[i]->IsActive()) {
			_entities[i]->Update(dt);
		}/*
		else {
			//deactivate / remove the entity;
			_entities.erase(_entities.begin() + i);
		}*/
	}
}

