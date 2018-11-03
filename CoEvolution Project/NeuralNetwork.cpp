#include "NeuralNetwork.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>

NeuralNetwork::NeuralNetwork(std::vector<int> nodesPerLayer, std::string filePath): _filePath(filePath)
{
	_nodeLayer = std::vector<std::vector<Node>>();
	_connectionLayer = std::vector<std::vector<NodeConnection>>();
	_epoch = 0;

	if (nodesPerLayer.size() < 3) {
		std::cout << "There are not enought layers to make network" << std::endl;
	}
	
	//init nodes and layers
	for (int i = 0; i < (int)nodesPerLayer.size(); i++) {
		_nodeLayer.push_back(std::vector<Node>());
		for (int j = 0; j < nodesPerLayer[i]; j++) {
			_nodeLayer[i].push_back(Node(0.5f));
		}
	}

	//connect the nodes up in each layer to the next excluding the last layer
	for (int i = 0; i < (int)nodesPerLayer.size() - 1; i++) {
		_connectionLayer.push_back(std::vector<NodeConnection>());
		for (int j = 0; j < nodesPerLayer[i]; j++) {
			for (int k = 0; k < nodesPerLayer[i + 1]; k++) {
				//make a connection from the current node to all the nodes in the next layer;
				NodeConnection c = NodeConnection(
					&_nodeLayer[i][j],
					&_nodeLayer[i + 1][k],
					((float(rand()) / float(RAND_MAX)) * (1 - (-1))) + (-1));
				//push the connection onto the the correct layer.
				_connectionLayer[i].push_back(c);
			}
		}
	}
}

NeuralNetwork::NeuralNetwork(std::string filePath): _filePath(filePath)
{
	//we can load a nn from a text file that has been saved. all it will be is the 

	std::ifstream file;
	std::string line;
	
	file.open(_filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ',')) {
				
				//load the file and read each line and out that stuff in the node and connection vectors
			}
		}
	}
}

std::string NeuralNetwork::ToString()
{
	
	std::string networkString = "";
	//what is the epoch and the topology of the network
	networkString.append(std::to_string(_epoch) + "\n");
	for (int i = 0; i < (int)_nodeLayer.size();i++) {
		networkString.append(std::to_string(_nodeLayer[i].size()));
		if (i != (int)_nodeLayer.size() - 1) {
			networkString.append(",");
		}
	}
	networkString.append("\n");
	//all the wieghts of the connections in order

	for (int i = 0; i < (int)_connectionLayer.size(); i++) {
		for (int j = 0; j < (int)_connectionLayer[i].size(); j++) {
			networkString.append(std::to_string(_connectionLayer[i][j].GetWeight()));
			if (j != (int)_connectionLayer[i].size() - 1) {
				networkString.append(",");
			}
		}
		networkString.append("\n");
	}

	return networkString;
}

void NeuralNetwork::SaveNetwork(std::string filePath)
{

}
