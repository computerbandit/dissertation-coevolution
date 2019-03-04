#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{
	//fill the layers with buckets
	for (int i = 0; i <= int(BACKGROUND_LAYER); i ++) {
		std::vector<IEntity*> layer = std::vector<IEntity*>();
		this->_entities.emplace(static_cast<ObjectLayer>(i), layer);
	}
}

GameObjectManager::~GameObjectManager()
{
	clearEntities();
}

void GameObjectManager::addEntity(IEntity * e, ObjectLayer layer)
{
	this->_entities.at(layer).push_back(e);
}

void GameObjectManager::removeEntity(IEntity * e, ObjectLayer layer)
{
	auto it = std::find(this->_entities.at(layer).begin(), this->_entities.at(layer).end(), e);
	if (it != this->_entities.at(layer).end()) { this->_entities.at(layer).erase(it); }
}


void GameObjectManager::clearEntities()
{
	for (int i = 0; i <= int(BACKGROUND_LAYER); i++) {
		this->_entities.at(static_cast<ObjectLayer>(i)).clear();
	}
}

void GameObjectManager::clearEntitiesInLayer(ObjectLayer layer)
{
	this->_entities.at(layer).clear();
}


void GameObjectManager::draw(float dt)
{
	for (int i = 0; i <= int(BACKGROUND_LAYER); i++) {
		for (IEntity* e : this->_entities.at(static_cast<ObjectLayer>(i))) {
			if (e->isActive()) {
				e->draw(dt);
			}
		}
	}
}

void GameObjectManager::update(float dt)
{
	for (int i = 0; i <= int(BACKGROUND_LAYER); i++) {
		for (IEntity* e : this->_entities.at(static_cast<ObjectLayer>(i))) {
			if (e->isActive()) {
				e->update(dt);
			}
			else if (e->toBeDestroyed()) {
				//deactivate / remove the entity;
				this->removeEntity(e, static_cast<ObjectLayer>(i));
			}
		}
	}
}
