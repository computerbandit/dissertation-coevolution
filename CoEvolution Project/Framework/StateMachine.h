#pragma once

#include <memory>
#include <stack>

#include "State.h"
//defines for state transtion flag, this is to the states can have some comuication between them, useful for pause menus and buttons and such
#define NEUTRAL '0'
#define FLAG_1 '1'
#define FLAG_2 '2'
#define FLAG_3 '3'
#define FLAG_4 '4'
#define FLAG_5 '5'
#define FLAG_6 '6'
#define FLAG_7 '7'
#define FLAG_8 '8'
#define EXIT '9' 


typedef std::unique_ptr<State> StateRef;

class StateMachine {
public:
	StateMachine() {}
	~StateMachine() {}

	void pushState(StateRef newState, bool isReplacing = true);
	void popState();

	void processStateChanges();

	void setSTF(char flag);
	void resetSTF();
	char& getSTF();

	StateRef& getAvtiveState();

private:
	std::stack<StateRef> _states;
	StateRef _newState;

	char _stateTransitionFlag = NEUTRAL;
	bool _isRemoving;
	bool _isAdding;
	bool _isRelplacing;
};