#include "NeuralNetworkGA.h"
#include <algorithm>
#include <iostream>
#include <string>



NeuralNetworkGA::NeuralNetworkGA(std::vector<NeuralNetwork> population, float mRate): _population(population), _mutationRate(mRate)
{
	_populationSize = (int)_population.size();
}

void NeuralNetworkGA::run(std::vector<float> input)
{
	for (NeuralNetwork& network : _population) {
		network.run(input);
	}
}

std::vector<NeuralNetwork>& NeuralNetworkGA::getPopulation()
{
	return _population;
}

const int & NeuralNetworkGA::getGeneration() const
{
	return _generation;
}

void NeuralNetworkGA::solved()
{
	this->_solution = true;
}

const bool & NeuralNetworkGA::isSolved() const
{
	return _solution;
}

void NeuralNetworkGA::setMutationRate(float mRate)
{
	this->_mutationRate = mRate;
}

const float & NeuralNetworkGA::getMutationRate() const
{
	return this->_mutationRate;
}

//given the current population pick a parent based on the fitnessRatio.
NeuralNetwork & NeuralNetworkGA::selectParent()
{
	float random = NeuralNetwork::randomFloat(0.0f, 1.0f);
	float theta = 0.0f;
	for (NeuralNetwork& network : _population) {
		theta += network.getFitnessRatio();
		if (random <= theta && !network.isSelected()) {
			network.setSelected(true);
			return network;
		}else if(network.getFitnessRatio() == 0.0f){
			return _population.back();
		}
	}
}
//given the new fitness of the networks work out the fitnessRatio for each network
void NeuralNetworkGA::evalutePopulation()
{
	//std::cout << "Evaluating Generation [" << _generation << "] ... ";
	float sum = 0.0f;
	for (NeuralNetwork& n : _population) {
		sum += n.getFitnessScore();
	}
	//set the fitness ratio for the selection process
	
	for (NeuralNetwork& n : _population) {
		if (sum != 0.0f) {
			n.setFitnessRatio(n.getFitnessScore() / sum);
		}
		else {
			n.setFitnessRatio(0.0f);
		}
	}
	//sort based on the fitnessRatio
	std::sort(_population.begin(), _population.end(), [](const NeuralNetwork& lhs, const NeuralNetwork& rhs)
	{
		return lhs.getFitnessRatio() > rhs.getFitnessRatio();
	});
	//std::cout << "DONE, Population Sorted" << std::endl;
}

//Generate the new population
void NeuralNetworkGA::nextGeneration()
{
	std::vector<NeuralNetwork> nextgen = std::vector<NeuralNetwork>();

	//std::cout << "Generating next generation ... ";
	for (int i = 0; i < (_populationSize/2); i++) {
		//crossover
		CrossoverProduct child = Crossover(selectParent(), selectParent());

		//make sure the the networks that were selected can be selected in the next loop

		for (NeuralNetwork& n : _population) {
			n.setSelected(false);
		}

		//mutate products
		if (NeuralNetwork::randomFloat(0.0f, 1.0f) <= this->_mutationRate) {
			mutate(child.A);
		}
		if (NeuralNetwork::randomFloat(0.0f, 1.0f) <= this->_mutationRate) {
			mutate(child.B);
		}

		nextgen.push_back(child.A);
		nextgen.push_back(child.B);
	}
	_population = nextgen;
	//std::cout << "DONE Generating" << std::endl;
	_generation++;
}

//Change the networks connection weights in a way to add diversity to the population and new unique solutions.
void NeuralNetworkGA::mutate(NeuralNetwork & network)
{
	//given the matrices that make up the network, change the values connecting each layer slightly.
	//convert the matrices into a chromeosome and the alter the values that way.
	//
	std::vector<Matrix> layers = network.getLayers();

	int numOfChanges = NeuralNetwork::randomInt(1, 10);

	for (int i = 0; i < numOfChanges; i++) {
		//pick a random weight in the network
		int layernum = NeuralNetwork::randomInt(0, layers.size() - 1);
		int row = NeuralNetwork::randomInt(0, layers[layernum].size() - 1);
		int coloumn = NeuralNetwork::randomInt(0, layers[layernum][row].size() - 1);

		//mutate this random weight that we have selected
		float& wieght = layers[layernum][row][coloumn];
		wieght += NeuralNetwork::randomFloatNromalDist(0.0f, 0.2f);
		if (wieght > 1.0f || wieght < -1.0f) {
			wieght = std::max(-1.0f, std::min(wieght, 1.0f));
		}
	}
	network.setLayers(layers);
}

CrossoverProduct NeuralNetworkGA::Crossover(NeuralNetwork & A,NeuralNetwork & B)
{
	std::vector<float> chromeosomeA = A.matricesToChromesome();
	std::vector<float> chromeosomeB = B.matricesToChromesome();

	int connections = (int)chromeosomeA.size();

	std::vector<float> newChromeosomeA = std::vector<float>(connections);
	std::vector<float> newChromeosomeB = std::vector<float>(connections);

	int numOfCrossoverPoints = 2;

	std::vector<int> crossoverPoints = std::vector<int>(numOfCrossoverPoints);

	for (int i = 0; i < numOfCrossoverPoints; i++) {
		crossoverPoints[i] = NeuralNetwork::randomInt(0,connections-1);
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

	NeuralNetwork childA = NeuralNetwork(A.getTopology(), newChromeosomeA);
	NeuralNetwork childB = NeuralNetwork(B.getTopology(), newChromeosomeB);
	CrossoverProduct product = CrossoverProduct(childA, childB);
	return product;
}

void NeuralNetworkGA::saveFittestNetwork(std::string token)
{
	fittestNetwork().saveNetwork(token);
}

float NeuralNetworkGA::averageFitness()
{
	float average = 0.0f;
	for (NeuralNetwork network : _population)
	{
		average += network.getFitnessScore();
	}
	average /= _populationSize;
	return average;
}

const NeuralNetwork & NeuralNetworkGA::fittestNetwork()
{
	return _population.front();
}
