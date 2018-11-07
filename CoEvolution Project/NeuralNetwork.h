#pragma once
#include <vector>
#include "Node.h"
#include "ConnectionWeight.h"

typedef std::vector<Node> NodeArray;
typedef std::vector<NodeArray> NodeNetwork;
typedef std::vector<ConnectionWeight> ConnectionArray;
typedef std::vector<ConnectionArray> ConnectionNetwork;
typedef unsigned int Epoch;

class NeuralNetwork {
public:

	NeuralNetwork(std::vector<int> nodesPerLayer, std::string filePath);
	NeuralNetwork(std::string filePath);
	~NeuralNetwork() {}

	std::string ToString();
	void SaveNetwork(std::string filePath = "");

	std::vector<float> Update(std::vector<float> inputs, bool train = false);


private:
	void BuildNetwork(std::vector<int> nodesPerLayer, std::vector<std::vector<float>> layersOfWeights);

	void AddLayer(const NodeArray& nodeArray, std::vector<float> weights);



	float RandomNumber(float Min, float Max);

	NodeNetwork _nodeNetwork;
	ConnectionNetwork _connectionNetwork;
	Epoch _epoch;
	std::string _filePath;
};