#pragma once

class State {
public:	
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() {}
	virtual void Resume() {}

	virtual void HandleEvents() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(float dt) = 0;
};