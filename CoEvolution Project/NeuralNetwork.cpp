#include "NeuralNetwork.h"

#include <iostream>
NeuralNetwork::NeuralNetwork(std::vector<int> nodesPerLayer)
{
	_nodeLayer = std::vector<std::vector<Node>>();
	_connectionLayer = std::vector<std::vector<NodeConnection>>();

	if (nodesPerLayer.size() < 3) {
		std::cout << "There are not enought layers to make network" << std::endl;
	}

	//init nodes and layers
	for (int i = 0; i < (int)nodesPerLayer.size(); i++) {
		for (int j = 0; j < nodesPerLayer[i]; j++) {
			_nodeLayer[i] = std::vector<Node>();
			_nodeLayer[i].push_back(Node(0.5f));
		}
	}

	//connect the nodes up in each layer to the next

	for (int i = 0; i < (int)nodesPerLayer.size() - 1; i++) {
		_connectionLayer[i] = std::vector<NodeConnection>();
		for (int j = 0; j < nodesPerLayer[i]; j++) {
			//make a connection from the current node to all the nodes in the next layer;

			for (int k = 0; k < nodesPerLayer[i+1]; k++) {
				//make a connection from the current node to all the nodes in the next layer;
				NodeConnection c = NodeConnection(
					&_nodeLayer[i][j],
					&_nodeLayer[i + 1][k],
					0.5f);
				//push the connection onto the the correct layer.
				_connectionLayer[i].push_back(c);
			}
		}
	}
}

void NeuralNetwork::PrintNetwork()
{
	for (std::vector<Node> layer : _nodeLayer) {
		for (Node n : layer) {
			std::cout << n.GetActivationFunction() << ", ";
		}
		std::cout << "\n";
	}
}
