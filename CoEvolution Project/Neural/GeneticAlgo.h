#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "NeuralNetwork.h"
#include "../Framework/Level.h"

struct CrossoverProduct
{
	NeuralNetwork A, B;
	CrossoverProduct(NeuralNetwork a, NeuralNetwork b) :A(a), B(b) {};
};

template <class T>
class GeneticAlgo{

	std::vector<T> _population;
	int _populationSize = 0, _generation = 0;
	float _mutationRate = 0.999f;
	bool _solution = false;
	std::string _gaData = "";

public:

	GeneticAlgo() {}
	GeneticAlgo(std::vector<T> population, float mRate, std::vector<std::string> extraData);
	~GeneticAlgo() {}
	
	std::vector<T>& getPopulation();
	const int& getGeneration()const;

	void solved();
	const bool& isSolved() const;

	void setMutationRate(float mRate);
	const float& getMutationRate() const;

	T& selectParent();
	void evalutePopulation();
	float sumPopulationScores();
	void setPopulationFitnessRatios(float sum);
	void sortPopulation();
	void nextGeneration();
	
	
	//Crossover and muation for levels


	//crossover and mutation for Networks
	void run(std::vector<float> input);
	void mutate(NeuralNetwork& network);
	CrossoverProduct crossover(NeuralNetwork& A, NeuralNetwork& B);

	void saveFittestInPopulation(std::string token);
	void savePopulation(std::string token);
	float averageFitness();
	const T& fittestInPopulation();

	int numberOfPopAboveFitness(float fitnessmarker);
	void saveGAData(std::string token);
};



template <class T>
GeneticAlgo<T>::GeneticAlgo(std::vector<T> population, float mRate, std::vector<std::string> extraData) : _population(population), _mutationRate(mRate)
{
	_populationSize = (int)_population.size();
	this->_gaData.append("Training Levels:,");
	for (std::string lname : extraData) {
		this->_gaData.append(lname + ",");
	}
	this->_gaData.append("\n");
	this->_gaData.append("Gen Num , Highest Fitness, Average Fitness, Population Size: " + std::to_string(_populationSize) + "\n");


}

template <class T>
void GeneticAlgo<T>::run(std::vector<float> input)
{
	if (dynamic_cast<NeuralNetwork>(T) != NULL) {
		for (T& network : _population) {
			network.run(input);
		}
	}
}

template <class T>
std::vector<T>& GeneticAlgo<T>::getPopulation()
{
	return _population;
}

template <class T>
const int & GeneticAlgo<T>::getGeneration() const
{
	return _generation;
}

template <class T>
void GeneticAlgo<T>::solved()
{
	this->_solution = true;
}

template <class T>
const bool & GeneticAlgo<T>::isSolved() const
{
	return _solution;
}

template <class T>
void GeneticAlgo<T>::setMutationRate(float mRate)
{
	this->_mutationRate = mRate;
}

template <class T>
const float & GeneticAlgo<T>::getMutationRate() const
{
	return this->_mutationRate;
}

template <class T>
//given the current population pick a parent based on the fitnessRatio.
T & GeneticAlgo<T>::selectParent()
{
	float random = NeuralNetwork::randomFloat(0.0f, 1.0f);
	float theta = 0.0f;
	for (T& a : _population) {
		theta += a.getFitnessRatio();
		if (random <= theta && !a.isSelected()) {
			a.setSelected(true);
			this->setPopulationFitnessRatios(this->sumPopulationScores());
			return a;
		}
		else if (a.getFitnessRatio() == 0.0f) {
			return _population.front();
		}
	}
	return _population.front();
}

//given the new fitness of the networks work out the fitnessRatio for each network
template <class T>
void GeneticAlgo<T>::evalutePopulation()
{
	//std::cout << "Evaluating Generation [" << _generation << "] ... ";

	//set the fitness ratio for the selection process
	this->setPopulationFitnessRatios(this->sumPopulationScores());
	this->sortPopulation();


	//TODO: After the population has been evaluated we need to export some of this data to th csv file, so we can look at the percentage error as the agents are lerning the levels

	float cappedFitness = this->fittestInPopulation().getFitness();
	cappedFitness = (cappedFitness >= 100.0f) ? 100.0f : cappedFitness;

	this->_gaData.append(std::to_string(this->getGeneration()) + " , " + std::to_string(cappedFitness) + " , " + std::to_string(this->averageFitness()) + " , " + std::to_string(this->numberOfPopAboveFitness(100.0f)) + "\n");

	//std::ofstream csvfile;
	//csvfile.open("Resources/networkdata/" +  + ".csv");


	//to make sure that the error of each of the generations 
}

template <class T>
float GeneticAlgo<T>::sumPopulationScores()
{
	float sum = 0.0f;
	for (T& a : _population) {
		if (!a.isSelected()) {
			sum += a.getFitness();
		}
	}
	return sum;
}

template <class T>
void GeneticAlgo<T>::setPopulationFitnessRatios(float sum)
{
	for (T& a : _population) {
		if (!a.isSelected()) {
			if (sum != 0.0f) {
				a.setFitnessRatio(a.getFitness() / sum);
			}
			else {
				a.setFitnessRatio(0.0f);
			}
		}
	}
}

template <class T>
void GeneticAlgo<T>::sortPopulation()
{
	//sort based on the fitnessRatio
	std::sort(_population.begin(), _population.end(), [](const T& lhs, const T& rhs)
	{
		return lhs.getFitnessRatio() > rhs.getFitnessRatio();
	});
}

//Generate the new population
template <class T>
void GeneticAlgo<T>::nextGeneration()
{
	std::vector<T> nextgen = std::vector<T>();

	//std::cout << "Generating next generation ... ";
	for (int i = 0; i < (_populationSize / 2); i++) {
		//crossover
		CrossoverProduct child = crossover(selectParent(), selectParent());

		//make sure the the networks that were selected can be selected in the next loop

		for (T& n : _population) {
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
template <class T>
void GeneticAlgo<T>::mutate(NeuralNetwork & network)
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

template <class T>
CrossoverProduct GeneticAlgo<T>::crossover(NeuralNetwork & A, NeuralNetwork & B)
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



	return CrossoverProduct(NeuralNetwork(A.getTopology(), newChromeosomeA, A.getExtraData()), NeuralNetwork(B.getTopology(), newChromeosomeB, B.getExtraData()));;
}

template <class T>
void GeneticAlgo<T>::saveFittestInPopulation(std::string token)
{
	if (dynamic_cast<NeuralNetwork>(T) != nullptr) {
		fittestInPopulation().saveNetwork(token);
	}/*
	else if (dynamic_cast<Level>(T) != nullptr) {

	}*/
}

template <class T>
void GeneticAlgo<T>::savePopulation(std::string token)
{
	for (int i = 0; i < _populationSize; i++) {
		this->_population.at(i).saveNetwork(token, std::to_string(i));
	}
}

template <class T>
float GeneticAlgo<T>::averageFitness()
{
	float average = 0.0f;
	for (NeuralNetwork network : _population)
	{
		average += network.getFitness();
	}
	average /= _populationSize;
	return average;
}

template <class T>
const T & GeneticAlgo<T>::fittestInPopulation()
{
	return _population.front();
}

template <class T>
int GeneticAlgo<T>::numberOfPopAboveFitness(float fitnessmarker)
{
	int count = 0;
	for (NeuralNetwork& n : this->_population) {
		if (n.getFitness() >= fitnessmarker) {
			count++;
		}
	}
	return count;
}

template <class T>
void GeneticAlgo<T>::saveGAData(std::string token)
{
	std::ofstream csv;
	if (csv.is_open()) {
		csv.close();
	}
	csv.open("Resources/networks/training-" + token + "/trainingdata.csv");
	csv << this->_gaData;
	csv.close();
}
