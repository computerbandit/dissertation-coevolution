#pragma once
#include <vector>
#include "Node.h"
#include "NodeConnection.h"

class NeuralNetwork {
public:

	NeuralNetwork(std::vector<int> nodesPerLayer, std::string filePath);
	NeuralNetwork(std::string filePath);
	~NeuralNetwork() {}

	std::string ToString();
	void SaveNetwork();

	std::vector<float> Update(std::vector<float> inputs, bool train = false);

private:
	std::vector<std::vector<Node>> _nodeLayer;
	std::vector<std::vector<NodeConnection>> _connectionLayer;

	std::string _filePath;
	int _epoch;
};