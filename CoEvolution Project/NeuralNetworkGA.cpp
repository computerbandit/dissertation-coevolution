#include "NeuralNetworkGA.h"
#include <algorithm>
#include <iostream>
#include <string>



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
NeuralNetwork & NeuralNetworkGA::SelectParent()
{
	float random = NeuralNetwork::RandomFloat(0.0f, 1.0f);
	float theta = 0.0f;
	for (NeuralNetwork& network : _population) {
		theta += network.GetFitnessRatio();
		if (random <= theta && !network.IsSelected()) {
			network.SetSelected(true);
			return network;
		}else if(network.GetFitnessRatio() == 0.0f){
			return _population.back();
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
	//set the fitness ratio for the selection process
	
	for (NeuralNetwork& n : _population) {
		if (sum != 0.0f) {
			n.SetFitnessRatio(n.GetFitnessScore() / sum);
		}
		else {
			n.SetFitnessRatio(0.0f);
		}
	}
	//sort based on the fitnessRatio
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

		for (NeuralNetwork& n : _population) {
			n.SetSelected(false);
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

	int numOfChanges = NeuralNetwork::RandomInt(1, 5);

	for (int i = 0; i < numOfChanges; i++) {
		//pick a random weight in the network
		int layernum = NeuralNetwork::RandomInt(0, layers.size() - 1);
		int row = NeuralNetwork::RandomInt(0, layers[layernum].size() - 1);
		int coloumn = NeuralNetwork::RandomInt(0, layers[layernum][row].size() - 1);

		//mutate this random weight that we have selected
		float& wieght = layers[layernum][row][coloumn];
		wieght += NeuralNetwork::RandomFloatNromalDist(0.0f, 0.2f);
		if (wieght > 1.0f || wieght < -1.0f) {
			wieght = std::max(-1.0f, std::min(wieght, 1.0f));
		}
	}
	network.SetLayers(layers);
}

CrossoverProduct NeuralNetworkGA::Crossover(NeuralNetwork & A,NeuralNetwork & B)
{
	std::vector<float> chromeosomeA = A.MatricesToChromesome();
	std::vector<float> chromeosomeB = B.MatricesToChromesome();

	int connections = (int)chromeosomeA.size();

	std::vector<float> newChromeosomeA = std::vector<float>(connections);
	std::vector<float> newChromeosomeB = std::vector<float>(connections);

	int numOfCrossoverPoints = NeuralNetwork::RandomInt(1, (int)A.GetTopology().size());

	std::vector<int> crossoverPoints = std::vector<int>(numOfCrossoverPoints);

	for (int i = 0; i < numOfCrossoverPoints; i++) {
		crossoverPoints[i] = NeuralNetwork::RandomInt(0,connections-1);
	}

	std::sort(crossoverPoints.begin(), crossoverPoints.end());
	crossoverPoints.erase(std::unique(crossoverPoints.begin(), crossoverPoints.end()), crossoverPoints.end());

	//perform the corssover on the chromeosomes
	//child A
	bool parentToggle = false;
	int index = 0;
	for (int i = 0; i < (int)chromeosomeA.size(); i++) {
		if (i == crossoverPoints[index]) {
			parentToggle = !parentToggle;
			index = (index + 1) % crossoverPoints.size();
		}
		if (parentToggle) {
			newChromeosomeA[i] = chromeosomeB[i];
			newChromeosomeB[i] = chromeosomeA[i];
		}
		else {
			newChromeosomeA[i] = chromeosomeA[i];
			newChromeosomeB[i] = chromeosomeB[i];
		}
	}

	NeuralNetwork childA = NeuralNetwork(A.GetTopology(), newChromeosomeA);
	NeuralNetwork childB = NeuralNetwork(B.GetTopology(), newChromeosomeB);
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
