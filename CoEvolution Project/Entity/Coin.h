#pragma once

#include "../Framework/IEntity.h"
#include "../Framework/AnimationController.h"
#include "../Framework/DEFINITIONS.h"
#include "../Framework/Game.h"
#include "../Framework/Level.h"

class Coin : public IEntity{
public:
	Coin(GameDataRef data);
	~Coin();


	// Inherited via IEntity
	virtual void init() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

	void collect();

private:
	GameDataRef _data;
	AnimationController* _animController;
};

