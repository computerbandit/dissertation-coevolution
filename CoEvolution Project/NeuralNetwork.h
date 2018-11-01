#pragma once
#include <vector>
#include "Node.h"
#include "NodeConnection.h"

class NeuralNetwork {
public:

	NeuralNetwork(std::vector<int> nodesPerLayer);
	~NeuralNetwork() {}

	void PrintNetwork();

private:
	std::vector<std::vector<Node>> _nodeLayer;
	std::vector<std::vector<NodeConnection>> _connectionLayer;

};