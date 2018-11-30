#include "NeuralNetworkGA.h"
#include "NeuralNetworkGA.h"
#include "NeuralNetworkGA.h"
#include <algorithm>
#include <iostream>
#include <string>

std::vector<Matrix> NeuralNetworkGA::ChromeosomeToMatrices(std::vector<int> topology, std::vector<float> chromeosome)
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

NeuralNetworkGA::NeuralNetworkGA(std::vector<NeuralNetwork> population, float mRate): _population(population), _mutationRate(mRate)
{
	_populationSize = (int)_population.size();
}

void NeuralNetworkGA::Run(std::vector<float> input)
{
	for (NeuralNetwork& network : _population) {
		network.Run(input);
	}
}

std::vector<NeuralNetwork>& NeuralNetworkGA::GetPopulation()
{
	return _population;
}

const int & NeuralNetworkGA::GetGeneration() const
{
	return _generation;
}

void NeuralNetworkGA::Solved()
{
	this->_solution = true;
}

const bool & NeuralNetworkGA::isSolved() const
{
	return _solution;
}

//given the current population pick a parent based on the fitnessRatio.
const NeuralNetwork & NeuralNetworkGA::SelectParent()
{
	float random = NeuralNetwork::RandomFloat(0.0f, 1.0f);
	float theta = 0.0f;
	for (NeuralNetwork& network : _population) {
		theta += network.GetFitnessRatio();
		if (random <= theta && !network.Selected()) {
			network.ToggleSelected();
			return network;
		}
	}
	return _population.back();
}
//given the new fitness of the networks work out the fitnessRatio for each network
void NeuralNetworkGA::EvalutePopulation()
{
	//std::cout << "Evaluating Generation [" << _generation << "] ... ";
	float sum = 0.0f;
	for (NeuralNetwork& n : _population) {
		sum += n.GetFitnessScore();
	}
	for (NeuralNetwork& n : _population) {
		n.SetFitnessRatio(n.GetFitnessScore() / sum);
	}
	//sort baes on the fitnessRatio
	std::sort(_population.begin(), _population.end(), [](const NeuralNetwork& lhs, const NeuralNetwork& rhs)
	{
		return lhs.GetFitnessRatio() > rhs.GetFitnessRatio();
	});
	//std::cout << "DONE, Population Sorted" << std::endl;
}

//Generate the new population
void NeuralNetworkGA::NextGeneration()
{
	std::vector<NeuralNetwork> nextgen = std::vector<NeuralNetwork>();


	//std::cout << "Generating next generation ... ";
	for (int i = 0; i < (_populationSize/2); i++) {
		//crossover
		CrossoverProduct child = Crossover(SelectParent(), SelectParent());

		//make sure the the networks that were selected can be selected in the next loop
		int j = 0;
		for (NeuralNetwork& n : _population) {
			if (n.Selected()) {
				n.ToggleSelected();
				j++;
			}
			if (j >= 2) {
				break;
			}
		}

		//mutate products
		if (NeuralNetwork::RandomFloat(0.0f, 1.0f) <= _mutationRate) {
			Mutate(child.A);
		}
		if (NeuralNetwork::RandomFloat(0.0f, 1.0f) <= _mutationRate) {
			Mutate(child.B);
		}

		nextgen.push_back(child.A);
		nextgen.push_back(child.B);
	}
	_population = nextgen;
	//std::cout << "DONE Generating" << std::endl;
	_generation++;
}
//Change the networks connection weights in a way to add diversity to the population and new unique solutions.
void NeuralNetworkGA::Mutate(NeuralNetwork & network)
{
	//given the matrices that make up the network, change the values connecting each layer slightly.
	//convert the matrices into a chromeosome and the alter the values that way.
	//
	std::vector<Matrix> layers = network.GetLayers();
	int connections = 0;
	for (Matrix m : layers) {
		for (std::vector<float> row : m) {
			connections += (int)row.size();
		}
	}
	int numOfChanges = NeuralNetwork::RandomInt(1, 1);

	for (int i = 0; i < numOfChanges; i++) {
		//pick a random weight in the network
		int layernum = NeuralNetwork::RandomInt(0, layers.size() - 1);
		int row = NeuralNetwork::RandomInt(0, layers.at(layernum).size() - 1);
		int coloumn = NeuralNetwork::RandomInt(0, layers.at(layernum).at(row).size() - 1);

		//mutate this random weight that we have selected
		layers.at(layernum).at(row).at(coloumn) += NeuralNetwork::RandomFloatNromalDist(0.0f, 0.2f);
	}
	network.SetLayers(layers);
}

CrossoverProduct NeuralNetworkGA::Crossover(const NeuralNetwork & A, const NeuralNetwork & B)
{
	std::vector<float> chromeosomeA = A.MatricesToChromesome();
	std::vector<float> chromeosomeB = B.MatricesToChromesome();
	std::vector<float> newChromeosomeA = std::vector<float>();
	std::vector<float> newChromeosomeB = std::vector<float>();

	int connections = (int)chromeosomeA.size();

	int numOfCrossoverPoints = NeuralNetwork::RandomInt(1, 3);

	std::vector<int> crossoverPoints = std::vector<int>();

	for (int i = 0; i < numOfCrossoverPoints; i++) {
		crossoverPoints.push_back(NeuralNetwork::RandomInt(0,connections-1));
	}

	std::sort(crossoverPoints.begin(), crossoverPoints.end());
	crossoverPoints.erase(std::unique(crossoverPoints.begin(), crossoverPoints.end()), crossoverPoints.end());

	//perform the corssover on the chromeosomes
	//child A
	bool parentToggle = false;
	int index = 0;
	for (int i = 0; i < (int)chromeosomeA.size(); i++) {
		if (i == crossoverPoints.at(index)) {
			parentToggle = !parentToggle;
			index = (index + 1) % crossoverPoints.size();
		}
		if (parentToggle) {
			newChromeosomeA.push_back(chromeosomeB.at(i));
			newChromeosomeB.push_back(chromeosomeA.at(i));
		}
		else {
			newChromeosomeA.push_back(chromeosomeA.at(i));
			newChromeosomeB.push_back(chromeosomeB.at(i));
		}
	}

	NeuralNetwork childA = NeuralNetwork(NeuralNetworkGA::ChromeosomeToMatrices(A.GetTopology(), newChromeosomeA));
	NeuralNetwork childB = NeuralNetwork(NeuralNetworkGA::ChromeosomeToMatrices(B.GetTopology(), newChromeosomeB));
	CrossoverProduct product = CrossoverProduct(childA, childB);

	return product;
}

void NeuralNetworkGA::SaveFittestNetwork()
{
	FittestNetwork().SaveNetwork();
}

float NeuralNetworkGA::AverageFitness()
{
	float average = 0.0f;
	for (NeuralNetwork network : _population)
	{
		average += network.GetFitnessScore();
	}
	average /= _populationSize;
	return average;
}

const NeuralNetwork & NeuralNetworkGA::FittestNetwork()
{
	return _population.front();
}
