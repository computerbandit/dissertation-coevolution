#include "NeuralNetwork.h"
#include <random>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>

NeuralNetwork::NeuralNetwork(std::vector<int> nodesPerLayer, std::string filePath): _filePath(filePath)
{
	if ((int)nodesPerLayer.size() < 2) {
		std::cout << "can't make a network out of the nodes that you have given" << std::endl;
	}
	
	this->_nodeNetwork = NodeNetwork();
	this->_connectionNetwork = ConnectionNetwork();
	this->_epoch = 0;

	std::vector<std::vector<float>> randWeights = std::vector<std::vector<float>>();

	for (int i = 1; i < (int)nodesPerLayer.size(); i++) {
		randWeights.push_back(std::vector<float>());
		for (int j = 0; j < nodesPerLayer[i-1] * nodesPerLayer[i]; j++){
			randWeights[i-1].push_back(this->RandomNumber(-0.1f, 0.1f));
		}
	}
	//build the network with the nodes this format and the weights given
	this->BuildNetwork(nodesPerLayer, randWeights);
}

NeuralNetwork::NeuralNetwork(std::string filePath): _filePath(filePath)
{
	this->_nodeNetwork = NodeNetwork();
	this->_connectionNetwork = ConnectionNetwork();
	this->_epoch = 0;

	std::ifstream file;
	std::string line;
	file.open(_filePath);
	int linenum = 0;
	std::vector<int> nodesPerLayer;
	std::vector<std::vector<float>> fileWeights = std::vector<std::vector<float>>();
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ',')) {
				if (linenum == 0) {
					_epoch = std::stoi(token);
				}
				else if(linenum == 1){
					nodesPerLayer.push_back(std::stoi(token));
				}
				else {
					fileWeights[linenum - 2].push_back(std::stof(token));
				}
			}
			linenum++;
			if (linenum > 1) {
				fileWeights.push_back(std::vector<float>());
			}
		}
	}
	
	//init network nodes and weights
	this->BuildNetwork(nodesPerLayer, fileWeights);
}

std::string NeuralNetwork::ToString()
{
	std::string networkString = "";
	//what is the epoch and the topology of the network
	networkString.append(std::to_string(_epoch) + "\n");
	for (int i = 0; i < (int)_nodeNetwork.size();i++) {
		networkString.append(std::to_string(_nodeNetwork[i].size()));
		if (i != (int)_nodeNetwork.size() - 1) {
			networkString.append(",");
		}
	}
	networkString.append("\n");
	//all the wieghts of the connections in order

	for (int i = 0; i < (int)_connectionNetwork.size(); i++) {
		for (int j = 0; j < (int)_connectionNetwork[i].size(); j++) {
			networkString.append(std::to_string(_connectionNetwork[i][j].GetWeight()));
			if (j != (int)_connectionNetwork[i].size() - 1) {
				networkString.append(",");
			}
		}
		networkString.append("\n");
	}

	return networkString;
}

void NeuralNetwork::SaveNetwork(std::string filePath)
{
	std::ofstream file;
	file.open((filePath == "")? this->_filePath: filePath);
	file << this->ToString();
	file.close();
}

/*This function will take in an set of inputs then pass them to the input layer and feed forward to the next until its at the output layer and those values will be returned.

	if the train bool is active then the after the parse the network will alter itself based on the something that has changed.
*/
std::vector<float> NeuralNetwork::Update(std::vector<float> inputs, bool train)
{
	std::vector<float> output = std::vector<float>();

	//first pass the inputs to the first layer of the network
	for (int i = 0; i < (int)_nodeNetwork[0].size(); i++) {
		_nodeNetwork[0][i].SetRawInput(inputs[i]);
	}

	//here the feed forward process beings
	for (int i = 1; i < (int)_nodeNetwork.size(); i++) {
		for (int j = 0; j < (int)_nodeNetwork[i].size(); j++)
		{
			_nodeNetwork[i][j].GenOutput();
			output.push_back(_nodeNetwork[i][j].Output());
		}
		//clear the output unless its that last layer
		if (i != (int)_nodeNetwork.size() - 1) {	
			output.clear();
		}
	}

	if (train) {
		_epoch++;
	}
	return output;
}

void NeuralNetwork::BuildNetwork(std::vector<int> nodesPerLayer, std::vector<std::vector<float>> weights)
{
	weights.insert(weights.begin(), std::vector<float>());
	for (int i = 0; i < (int)nodesPerLayer.size(); i++) {
		//we need to make the NodeArray based on the size of each layer
		NodeArray layer = NodeArray();
		for (int j = 0; j < nodesPerLayer[i]; j++) {
			if (i == (int)nodesPerLayer.size() - 1) {
				layer.push_back(Node(ActivationFunction::HARDLIM, 0.5f));
			}
			else {
				layer.push_back(Node(ActivationFunction::SIGMOID, 0.0f));
			}
		}
		//after we have made the layer we need to add to the network.
		AddLayer(layer, weights[i]);
	}
}

/*This function adds the next layer of the network to the main framework, this will link the new layer with the previous layer if it's not the input layer. This also handles how the network is like a doubly linked list between all the weighted connections. This is because the */
void NeuralNetwork::AddLayer(const NodeArray& nodeArray, std::vector<float> weightValues)
{
	_nodeNetwork.push_back(nodeArray);
	//if the network has more than one layer then 
	if ((int)_nodeNetwork.size() > 1) {
		//now we need to make the connectionWeight create the and then assign the 
		//we need to pass pointers of the nodes that are to be connected (next and prev Node).

		NodeArray& prevLayer = _nodeNetwork[_nodeNetwork.size() - 2];
		NodeArray& currentLayer = _nodeNetwork[_nodeNetwork.size() - 1];

		ConnectionArray connectionLayer = ConnectionArray();
		int i = 0;
		for (Node& cnode : currentLayer) {
			for (Node& pnode : prevLayer) {
				ConnectionWeight c(&cnode, &pnode, weightValues[i++]);
				connectionLayer.push_back(c);
			}
		}
		_connectionNetwork.push_back(connectionLayer);
		i = 0;
		for (Node& cnode : currentLayer) {
			for (int j = 0; j < (int)prevLayer.size(); j++) {
				cnode.AssignInput(&(_connectionNetwork.back()[i++]));
			}
		}
	}
}


float NeuralNetwork::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
