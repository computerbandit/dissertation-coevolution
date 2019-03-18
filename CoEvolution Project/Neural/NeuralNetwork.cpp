#include "NeuralNetwork.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

float NeuralNetwork::sigmoid(float sum, float theta)
{
	return (std::exp(sum - theta) - std::exp(-(sum - theta))) / (std::exp(sum - theta) + std::exp(-(sum - theta)));
}

float NeuralNetwork::hardLimit(float sum, float theta)
{
	if (sum >= theta) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}


NeuralNetwork::NeuralNetwork(std::vector<int> topology) {

	this->_networkId = newNetworkId();
	_layer = std::vector<Matrix>();
	for (int i = 1; i < (int)topology.size(); i++) {
		Matrix m = Matrix();
		for (int j = 0; j < topology.at(i-1); j++) {
			std::vector<float> row = std::vector<float>();
			for (int k = 0; k < topology.at(i); k++) {
				//add gaussian distribution or something 
				row.push_back(0.0f);
			}
			m.push_back(row);
		}
		_layer.push_back(m);
	}
	
	 this->setChromeosome(this->matricesToChromesome());
}


NeuralNetwork::NeuralNetwork(std::string filePath) 
{
	this->_networkId = newNetworkId();
	_layer = std::vector<Matrix>();
	_extraData = std::vector<std::string>();
	std::ifstream file;
	std::string line;
	file.open(filePath);
	int coloumn = 0;
	bool dataline = false;
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			
			while (std::getline(ss, token, ',')) {
				if (token == "#") {
					_layer.push_back(Matrix());
				}
				else if(token.compare(0, 5, "#Data") == 0) {
					dataline = true;
				}else if(dataline){
					this->_extraData.push_back(token);
				}
				else {
					if (coloumn == 0) {
						_layer.back().push_back(std::vector<float>());
					}
					_layer.back().back().push_back(std::stof(token));
					coloumn++;
				}
			}
			coloumn = 0;
		}
	}
	else {
		throw "Network File not found";
	}
}

NeuralNetwork::NeuralNetwork(std::vector<int> topology, std::vector<float> chromeosome, std::vector<std::string> extraData): _extraData(extraData)
{
	this->setChromeosome(chromeosome);

	this->_networkId = newNetworkId();
	_layer = NeuralNetwork::chromeosomeToMatrices(topology, _chromeosome);
}

std::string NeuralNetwork::toString() const
{
	std::string layersString = "";
	

	for (int k = 0; k < (int)_layer.size(); k++) {
		layersString.append("#\n");
		for (int j = 0; j < (int)_layer.at(k).size(); j++) {
			for (int i = 0; i < (int)_layer.at(k).at(j).size(); i++) {
				layersString.append(std::to_string(_layer.at(k).at(j).at(i)) + ((i == (int)_layer.at(k).at(j).size() - 1)? "" : ","));
			}
			layersString.append("\n");
		}
	}

	layersString.append("#Data\n");
	for (int i = 0; i < int(_extraData.size()); i++) {
		layersString.append(_extraData.at(i));
		if (i < int(_extraData.size()) - 1) {
			layersString.append(",");
		}
	}

	return layersString;
}

//given a set of inputs what is the output of the network

std::vector<float> NeuralNetwork::matrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function)
{
	_output = std::vector<float>((int)m[0].size());
	for (float& f: _output) {
		f = 0.0f;
	}

	for (int i = 0; i < (int)m.size(); i++) {
		for (int j = 0; j < (int)m.at(i).size(); j++) {
			_output.at(j) += m.at(i).at(j) * input.at(i);
		}
	}

	//now we need to apply the node function activation to the sum 
	for (float& output : _output) {
		switch (function)
		{
		case SIGMOID:
			output = this->sigmoid(output, 0.0f);
			break;
		case HARDLIM:
			output = this->hardLimit(output, 0.5f);
			break;
		default:
			break;
		}

	}

	return _output;
}


void NeuralNetwork::run(std::vector<float> input)
{	
	for (int i = 0 ; i < (int)_layer.size(); i++){
		input = this->matrixOutput(_layer.at(i), input, (i == (int)_layer.size()-1)? SIGMOID : SIGMOID);
	}
	_output = input;
}


const std::vector<float>& NeuralNetwork::getOutput() const
{
	return _output;
}


void NeuralNetwork::saveNetwork(std::string token, std::string fileName) const
{
	std::ofstream file;
	if (fileName == "") {
		for (int i = 0; i < (int)getTopology().size(); i++) {
			fileName.append(std::to_string(getTopology().at(i)) + ((i == (int)getTopology().size() - 1) ? "" : "_"));
		}
		file.open("Resources\\networks\\training-" + token + "\\" + fileName + "-" + std::to_string(_networkId) + ".net");
	}
	else {
		file.open("Resources\\networks\\training-" + token + "\\" + fileName + ".net");
	}
	
	file << this->toString();
	file.close();
}


std::vector<float> NeuralNetwork::matricesToChromesome() const
{
	std::vector<float> chromeosome = std::vector<float>();
	
	for (Matrix m : this->_layer) {
		for (std::vector<float> row : m) {
			for (float weight : row) {
				chromeosome.push_back(weight);
			}
		}
	}
	return chromeosome;
}


std::vector<int> NeuralNetwork::getTopology() const
{
	std::vector<int> topology = std::vector<int>();
	for (const Matrix& m : _layer) {
		topology.push_back(m.size());
	}
	topology.push_back(_layer.back().back().size());
	return topology;
}



std::vector<Matrix>& NeuralNetwork::getLayers() 
{
	return _layer;
}


void NeuralNetwork::setLayers(std::vector<Matrix> newLayers)
{
	_layer = newLayers;
}


const std::vector<std::string>& NeuralNetwork::getExtraData() const
{
	return this->_extraData;
}


void NeuralNetwork::addExtraData(std::vector<std::string> extraData)
{
	for (std::string s : extraData) {
		this->_extraData.push_back(s);
	}
}


void NeuralNetwork::setExtraData(std::vector<std::string> extraData)
{
	this->_extraData = extraData;
}



std::vector<Matrix> NeuralNetwork::chromeosomeToMatrices(std::vector<int> topology, std::vector<float> chromeosome)
{
	std::vector<Matrix> layers = std::vector<Matrix>();
	int chromeosomeNum = 0;
	for (int i = 1; i < (int)topology.size(); i++) {
		Matrix m = Matrix();
		for (int j = 0; j < topology.at(i - 1); j++) {
			std::vector<float> row = std::vector<float>();
			for (int k = 0; k < topology.at(i); k++) {
				row.push_back(chromeosome.at(chromeosomeNum++));
			}
			m.push_back(row);
		}
		layers.push_back(m);
	}
	return layers;
}


std::vector<NeuralNetwork> NeuralNetwork::generatePopulation(int populationSize, std::vector<int> topology)
{
	std::vector<NeuralNetwork>* population = new std::vector<NeuralNetwork>();
	std::cout << "Generating " + std::to_string(populationSize) + " networks. \n ---please wait---" << std::endl;

	for (int i = 0; i < populationSize; i++) {
			population->push_back(NeuralNetwork(topology));
	}

	std::cout << "-!!-DONE-!!-" << std::endl;
	return *population;
}


float NeuralNetwork::randomFloat(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}


int NeuralNetwork::randomInt(int Min, int Max)
{
	return Min + (rand() % static_cast<int>(Max - Min + 1));
}


float NeuralNetwork::randomFloatNromalDist(float mean, float stddev)
{
	static std::default_random_engine generator;
	std::normal_distribution<float> distribution(mean, stddev);
	return distribution(generator);
}


int NeuralNetwork::newNetworkId()
{
	return _networkCount++;
}


int NeuralNetwork::_networkCount = 0;