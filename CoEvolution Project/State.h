#pragma once

class State {
public:	
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() {}
	virtual void resume() {}

	virtual void handleEvents() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;
};