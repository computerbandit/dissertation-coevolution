#include "NeuralNetworkGA.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

NeuralNetworkGA::NeuralNetworkGA(std::vector<NeuralNetwork> population, float mRate): _population(population), _mutationRate(mRate)
{
	_populationSize = (int)_population.size();
	this->_gaData.append("Gen Num , Highest Fitness, Average Fitness, Population Size: " + std::to_string(_populationSize) + "\n");
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
			this->setPopulationFitnessRatios(this->sumPopulationScores());
			return network;
		}else if(network.getFitnessRatio() == 0.0f){
			return _population.front();
		}
	}
	return _population.front();
}
//given the new fitness of the networks work out the fitnessRatio for each network
void NeuralNetworkGA::evalutePopulation()
{
	//std::cout << "Evaluating Generation [" << _generation << "] ... ";

	//set the fitness ratio for the selection process
	this->setPopulationFitnessRatios(this->sumPopulationScores());
	this->sortPopulation();
	

	//TODO: After the population has been evaluated we need to export some of this data to th csv file, so we can look at the percentage error as the agents are lerning the levels

	this->_gaData.append(std::to_string(this->getGeneration()) +" , " + std::to_string(this->fittestNetwork().getFitnessScore()) + " , " + std::to_string(this->averageFitness()) + " , " + std::to_string(this->numberOfNNAboveFitness(100.0f)) + "\n");

	//std::ofstream csvfile;
	//csvfile.open("Resources/networkdata/" +  + ".csv");


	//to make sure that the error of each of the generations 
}

float NeuralNetworkGA::sumPopulationScores()
{
	float sum = 0.0f;
	for (NeuralNetwork& n : _population) {
		if (!n.isSelected()) {
			sum += n.getFitnessScore();
		}
	}
	return sum;
}

void NeuralNetworkGA::setPopulationFitnessRatios(float sum)
{
	for (NeuralNetwork& n : _population) {
		if (!n.isSelected()) {
			if (sum != 0.0f) {
				n.setFitnessRatio(n.getFitnessScore() / sum);
			}
			else {
				n.setFitnessRatio(0.0f);
			}
		}
	}
}

void NeuralNetworkGA::sortPopulation()
{
	//sort based on the fitnessRatio
	std::sort(_population.begin(), _population.end(), [](const NeuralNetwork& lhs, const NeuralNetwork& rhs)
	{
		return lhs.getFitnessRatio() > rhs.getFitnessRatio();
	});
}

//Generate the new population
void NeuralNetworkGA::nextGeneration()
{
	std::vector<NeuralNetwork> nextgen = std::vector<NeuralNetwork>();

	//std::cout << "Generating next generation ... ";
	for (int i = 0; i < (_populationSize/2); i++) {
		//crossover
		CrossoverProduct child = crossover(selectParent(), selectParent());

		//make sure the the networks that were selected can be selected in the next loop

		for (NeuralNetwork& n : _population) {
			n.setSelected(false);
		}

		//mutate products
		mutate(child.A);
		mutate(child.B);

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

	for (Matrix& m : layers) {
		for (std::vector<float>& layer : m) {
			for (float& w : layer) {
				if (NeuralNetwork::randomFloat(0.0f, 1.0f) >= this->_mutationRate) {
					w += NeuralNetwork::randomFloatNromalDist(0.0f, 0.4f);
					if (w > 1.0f || w < -1.0f) {
						w = std::max(-1.0f, std::min(w, 1.0f));
					}
				}
			}
		}
	}

	network.setLayers(layers);
}

CrossoverProduct NeuralNetworkGA::crossover(NeuralNetwork & A,NeuralNetwork & B)
{
	std::vector<float> chromeosomeA = A.matricesToChromesome();
	std::vector<float> chromeosomeB = B.matricesToChromesome();

	int connections = (int)chromeosomeA.size();

	std::vector<float> newChromeosomeA = std::vector<float>(connections);
	std::vector<float> newChromeosomeB = std::vector<float>(connections);

	
	if (NeuralNetwork::randomFloat(0.0f, 1.0f) >= 0.90f) {
		//int numOfCrossoverPoints = NeuralNetwork::randomInt((chromeosomeA.size()-1)/4, (chromeosomeA.size() - 1) / 2);
		//int numOfCrossoverPoints = NeuralNetwork::randomInt(10, (chromeosomeA.size() - 1) / 2);

		int numOfCrossoverPoints = 1;
		std::vector<int> crossoverPoints = std::vector<int>();
		crossoverPoints.push_back(int(std::floor((chromeosomeA.size() - 1) / 2)));
		//randomly selects random points on the chromeosome and then sorts them
		/*
		for (int i = 0; i < numOfCrossoverPoints; i++) {
			crossoverPoints[i] = NeuralNetwork::randomInt(0, (connections - 1) < 0 ? connections : (connections - 1));
		}
		std::sort(crossoverPoints.begin(), crossoverPoints.end());
		crossoverPoints.erase(std::unique(crossoverPoints.begin(), crossoverPoints.end()), crossoverPoints.end());
		*/

		//perform the corssover on the chromeosomes
		//child A
		bool parentToggle = false;
		int index = 0;
		//1% change to average the wieghts

		if (NeuralNetwork::randomFloat(0.0f, 1.0f) >= 0.90f) {
			for (int i = 0; i < (int)chromeosomeA.size(); i++) {
				float sum = (chromeosomeB[i] + chromeosomeA[i]) / 2.0f;
				newChromeosomeA[i] = sum;
				newChromeosomeB[i] = sum;
			}
		}
		else {
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
		}
	}
	else {
		if (NeuralNetwork::randomFloat(0.0f, 1.0f) >= 0.50f) {
			newChromeosomeA = chromeosomeA;
			newChromeosomeB = chromeosomeA;
		}
		else {
			newChromeosomeA = chromeosomeB;
			newChromeosomeB = chromeosomeB;
		}
	}



	return CrossoverProduct(NeuralNetwork(A.getTopology(), newChromeosomeA), NeuralNetwork(B.getTopology(), newChromeosomeB));;
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

int NeuralNetworkGA::numberOfNNAboveFitness(float fitnessmarker)
{
	int count = 0;
	for (NeuralNetwork& n : this->_population) {
		if (n.getFitnessScore() >= fitnessmarker) {
			count++;
		}
	}
	return count;
}

void NeuralNetworkGA::saveGAData(std::string token)
{
	std::string topologyString = "";
	for (int i = 0; i < (int)this->_population.back().getTopology().size(); i++) {
		topologyString.append(std::to_string(this->_population.back().getTopology().at(i)) + ((i == (int)this->_population.back().getTopology().size() - 1) ? "" : "_"));
	}

	std::ofstream csv;
	csv.open("Resources/networkdata/" + topologyString + "-" + token + ".csv");
	csv << this->_gaData;
	csv.close();
}
