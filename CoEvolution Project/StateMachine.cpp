#include "StateMachine.h"

void StateMachine::PushState(StateRef newState, bool isReplacing)
{
	this->_isAdding = true;
	this->_isRelplacing = _isRelplacing;
	
	this->_newState = std::move(newState);
}

void StateMachine::PopState()
{
	this->_isRemoving = true;
}

void StateMachine::ProcessStateChanges()
{
	if (this->_isRelplacing && !this->_states.empty())
	{
		this->_states.pop();
		if (!this->_states.empty()) {
			this->_states.top()->Resume();
		}
		this->_isRemoving = false;
	}

	if (this->_isAdding) {
		if (!this->_states.empty()) {
			if (this->_isRelplacing) {
				this->_states.top()->Cleanup();
				this->_states.pop();
			}
			else {
				this->_states.top()->Pause();
			}
		}
		this->_states.push(std::move(this->_newState));
		this->_states.top()->Init();
		this->_isAdding = false;
	}
}

StateRef & StateMachine::GetAvtiveState()
{
	return this->_states.top();
}


