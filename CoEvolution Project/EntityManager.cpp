#include "EntityManager.h"

void EntityManager::Add(IEntity * e)
{
	this->_entities.push_back(e);
}


void EntityManager::Clear()
{
	this->_entities.clear();
}

void EntityManager::Draw(float dt)
{
	for (IEntity* e : _entities) {
		e->Draw(dt);
	}
}

void EntityManager::Update(float dt)
{
	for (IEntity* e : _entities) {
		e->Update(dt);
	}
}

