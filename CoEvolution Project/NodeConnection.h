#pragma once
#include "Node.h"

class NodeConnection {
public:

	NodeConnection(Node * next, Node * prev, float weight);
	~NodeConnection() {}

	float& GetWeight() {
		return _weight;
	}

private:
	Node * _next, * _prev;
	float _weight;
};
