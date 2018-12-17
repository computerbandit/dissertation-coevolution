#pragma once

#include <memory>
#include <stack>

#include "State.h"

typedef std::unique_ptr<State> StateRef;

class StateMachine {
public:
	StateMachine() {}
	~StateMachine() {}

	void pushState(StateRef newState, bool isReplacing = true);
	void popState();

	void processStateChanges();

	StateRef& getAvtiveState();

private:
	std::stack<StateRef> _states;
	StateRef _newState;

	bool _isRemoving;
	bool _isAdding;
	bool _isRelplacing;
};