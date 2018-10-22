#pragma once

#include <memory>
#include <stack>

#include "State.h"

typedef std::unique_ptr<State> StateRef;

class StateMachine {
public:
	StateMachine() {}
	~StateMachine() {}

	void PushState(StateRef newState, bool isReplacing = true);
	void PopState();

	void ProcessStateChanges();

	StateRef& GetAvtiveState();

private:
	std::stack<StateRef> _states;
	StateRef _newState;

	bool _isRemoving;
	bool _isAdding;
	bool _isRelplacing;
};