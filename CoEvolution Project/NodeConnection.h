#pragma once
#include "Node.h"

class NodeConnection {
public:

	NodeConnection(Node * a, Node * b, float weight);
	~NodeConnection() {}

	float& GetWeight() {
		return _weight;
	}

private:
	Node * A, * B;
	float _weight;
};
