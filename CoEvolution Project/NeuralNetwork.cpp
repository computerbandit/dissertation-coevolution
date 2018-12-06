#include "NeuralNetwork.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


float NeuralNetwork::Sigmoid(float sum, float theta)
{
	return (std::exp(sum - theta) - std::exp(-(sum - theta))) / (std::exp(sum - theta) + std::exp(-(sum - theta)));
}

float NeuralNetwork::HardLimit(float sum, float theta)
{
	if (sum >= theta) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}


NeuralNetwork::NeuralNetwork(std::vector<int> topology) {
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
	this->_chromeosome = this->MatricesToChromesome();
}


NeuralNetwork::NeuralNetwork(std::string filePath) 
{
	_layer = std::vector<Matrix>();

	std::ifstream file;
	std::string line;
	file.open(filePath);
	int coloumn = 0;
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ',')) {
				if (token == "#") {
					_layer.push_back(Matrix());
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
}

NeuralNetwork::NeuralNetwork(std::vector<int> topology, std::vector<float> chromeosome): _chromeosome(chromeosome)
{
	_layer = NeuralNetwork::ChromeosomeToMatrices(topology, _chromeosome);

}

std::string NeuralNetwork::ToString() const
{
	std::string layersString = "";
	for (int k = 0; k < (int)_layer.size(); k++) {
		layersString.append("#\n");
		for (int j = 0; j < (int)_layer.at(k).size(); j++) {
			for (int i = 0; i < (int)_layer.at(k).at(j).size(); i++) {
				layersString.append(std::to_string(_layer.at(k).at(j).at(i)) + ((i == (int)_layer.at(k).at(j).size() - 1)? "" : ","));
			}
			if ((k != (int)_layer.size()-1) || (j != (int)_layer.at(k).size() - 1)) {
				layersString.append("\n");
			}
		}
	}
	return layersString;
}

//given a set of inputs what is the output of the network
std::vector<float> NeuralNetwork::MatrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function)
{
	_output = std::vector<float>((int)m[0].size());
	for (float& f: _output) {
		f = 0.0f;
	}

	for (int i = 0; i < (int)m.size(); i++) {
		for (int j = 0; j < (int)m[i].size(); j++) {
			_output[j] += m[i][j] * input[i];
		}
	}

	//now we need to apply the node function activation to the sum 
	for (float& output : _output) {
		switch (function)
		{
		case SIGMOID:
			output = this->Sigmoid(output, 0.0f);
			break;
		case HARDLIM:
			output = this->HardLimit(output, 0.5f);
			break;
		default:
			break;
		}

	}

	return _output;
}

void NeuralNetwork::Run(std::vector<float> input)
{
	for (int i = 0 ; i < (int)_layer.size(); i++){
		input = this->MatrixOutput(_layer.at(i), input, (i == (int)_layer.size()-1)? HARDLIM : SIGMOID);
	}
	_output = input;
}

const std::vector<float>& NeuralNetwork::GetOutput() const
{
	return _output;
}

void NeuralNetwork::SaveNetwork(std::string token) const
{
	std::string topologyString = "";
	for (int i = 0; i < (int)GetTopology().size(); i++) {
		topologyString.append(std::to_string(GetTopology().at(i)) + ((i == (int)GetTopology().size() - 1) ? "" : "_"));
	}

	std::ofstream file;
	file.open("Resources/networks/" + topologyString + "-" + token + ".txt");
	file << this->ToString();
	file.close();
}

std::vector<float> NeuralNetwork::MatricesToChromesome() const
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

std::vector<int> NeuralNetwork::GetTopology() const
{
	std::vector<int> topology = std::vector<int>();
	for (const Matrix& m : _layer) {
		topology.push_back(m.size());
	}
	topology.push_back(_layer.back().back().size());
	return topology;
}

const float& NeuralNetwork::GetFitnessScore() const
{
	return _fitnessScore;
}

void NeuralNetwork::SetFitnessScore(float fitnessScore)
{
	_fitnessScore = fitnessScore;
}

const float& NeuralNetwork::GetFitnessRatio() const
{
	return _fitnessRatio;
}

void NeuralNetwork::SetFitnessRatio(float fitnessRatio)
{
	_fitnessRatio = fitnessRatio;
}


std::vector<Matrix>& NeuralNetwork::GetLayers() 
{
	return _layer;
}

void NeuralNetwork::SetLayers(std::vector<Matrix> newLayers)
{
	_layer = newLayers;
}

const bool & NeuralNetwork::IsSelected() const
{
	return _selected;
}

void NeuralNetwork::SetSelected(bool selected)
{
	_selected = !selected;
}


const std::vector<float>& NeuralNetwork::GetChromeosome() const
{
	return this->_chromeosome;
}

std::vector<Matrix> NeuralNetwork::ChromeosomeToMatrices(std::vector<int> topology, std::vector<float> chromeosome)
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

std::vector<NeuralNetwork> NeuralNetwork::GeneratePopulation(int populationSize, std::vector<int> topology)
{
	std::vector<NeuralNetwork>* population = new std::vector<NeuralNetwork>();
	std::cout << "Generating " + std::to_string(populationSize) + " networks. \n ---please wait---" << std::endl;

	for (int i = 0; i < populationSize; i++) {
			population->push_back(NeuralNetwork(topology));
	}

	std::cout << "-!!-DONE-!!-" << std::endl;
	return *population;
}

float NeuralNetwork::RandomFloat(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

int NeuralNetwork::RandomInt(int Min, int Max)
{
	return Min + (rand() % static_cast<int>(Max - Min + 1));
}

float NeuralNetwork::RandomFloatNromalDist(float mean, float stddev)
{
	static std::default_random_engine generator;
	std::normal_distribution<float> distribution(mean, stddev);
	return distribution(generator);
}

int NeuralNetwork::NewNetworkId()
{
	return _networkCount++;
}
