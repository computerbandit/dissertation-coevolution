#include "StateMachine.h"

void StateMachine::pushState(StateRef newState, bool isReplacing)
{
	this->_isAdding = true;
	this->_isRelplacing = isReplacing;
	
	this->_newState = std::move(newState);
}

void StateMachine::popState()
{
	this->_isRemoving = true;
}

void StateMachine::processStateChanges()
{
	if (this->_isRemoving && !this->_states.empty())
	{
		this->_states.top()->cleanup();
		this->_states.pop();
		if (!this->_states.empty()) {
			this->_states.top()->resume();
		}
		this->_isRemoving = false;
	}

	if (this->_isAdding) {
		if (!this->_states.empty()) {
			if (this->_isRelplacing) {
				this->_states.top()->cleanup();
				this->_states.pop();
			}
			else {
				this->_states.top()->pause();
			}
		}
		this->_states.push(std::move(this->_newState));
		this->_states.top()->init();
		this->_isAdding = false;
	}
}

StateRef & StateMachine::getAvtiveState()
{
	return this->_states.top();
}


