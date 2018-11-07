#pragma once
#include "Node.h"

class ConnectionWeight {
public:
	
	ConnectionWeight(Node * next, Node * prev, float weight);
	~ConnectionWeight() {}

	float& GetWeight() {
		return _weight;
	}

	Node& GetNextNode() {
		return *_next;
	}
	Node& GetPrevNode() {
		return *_prev;
	}

private:
	Node* _next, * _prev;
	float _weight;
};
