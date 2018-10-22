#pragma once
#include "IEntity.h"
#include "Game.h"

class Tile : virtual public IEntity{
public:
	Tile(GameDataRef data, int tileID, float tilesize, sf::Vector2i _mapsize, int index);
	~Tile() {}

	// Inherited via IEntity
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

public:
	GameDataRef _data;
	int _tileID;
	bool _static;
	sf::RectangleShape _rect;
};