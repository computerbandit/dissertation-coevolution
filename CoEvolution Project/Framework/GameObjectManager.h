#pragma once
#include <vector>
#include <map>
#include "IEntity.h"

enum ObjectLayer {
	BACKGROUND_LAYER, LEVEL_LAYER, COIN_LAYER, PLAYER_LAYER, ENEMEY_LAYER, TOP_LAYER
};

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	void addEntity(IEntity* e, ObjectLayer layer);
	void removeEntity(IEntity* e, ObjectLayer layer);
	void clearEntities();
	void clearEntitiesInLayer(ObjectLayer layer);

	void draw(float dt);
	void update(float dt);

	template <class T> std::vector<T*> collisionCheck(sf::FloatRect hitBox, ObjectLayer layer);

private:
	std::map<ObjectLayer, std::vector<IEntity*>> _entities;
};

template<class T>
inline std::vector<T*> GameObjectManager::collisionCheck(sf::FloatRect hitBox, ObjectLayer layer)
{
	std::vector<T*> entitesInArea = std::vector<T*>();
	T* eptr = nullptr;
	for (IEntity* e : this->_entities.at(layer)) {
		if ((eptr = dynamic_cast<T*>(e)) != NULL) {
			if (hitBox.intersects(e->getSprite().getGlobalBounds())) {
				entitesInArea.push_back(eptr);
			}
		}

	}
	return entitesInArea;
}
