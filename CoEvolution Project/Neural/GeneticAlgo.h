#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "NeuralNetwork.h"
#include "../Framework/Level.h"

template <class T>
struct CrossoverProduct
{
	T A, B;
	CrossoverProduct(T a, T b) :A(a), B(b) {};
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

	void mutate(Level& level);
	CrossoverProduct<T> crossover(Level& A, Level& B);


	//crossover and mutation for Networks
	void run(std::vector<float> input);
	void mutate(NeuralNetwork& network);
	CrossoverProduct<T> crossover(NeuralNetwork& A, NeuralNetwork& B);

	void saveFittestInPopulation(std::string path, std::string token, std::string subfolder, std::string fileName = "");
	void savePopulation(std::string path, std::string token, std::string subfolder);
	float averageFitness();
	T& fittestInPopulation();

	int numberOfPopAboveFitness(float fitnessmarker);
	void writeGAData(std::string path, std::string token, std::string subfolder);
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
	for (IFitness& a : _population) {
		theta += a.getFitnessRatio();
		if (random <= theta && !a.isSelected()) {
			a.setSelected(true);
			this->setPopulationFitnessRatios(this->sumPopulationScores());
			return (T&)a;
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

	float cappedFitness = ((IFitness)this->fittestInPopulation()).getFitness();
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
	for (IFitness & a : _population) {
		if (!a.isSelected()) {
			sum += a.getFitness();
		}
	}
	return sum;
}

template <class T>
void GeneticAlgo<T>::setPopulationFitnessRatios(float sum)
{
	for (IFitness& a : _population) {
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
	std::sort(_population.begin(), _population.end(), [](const IFitness& lhs, const IFitness& rhs)
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
		CrossoverProduct<T> child = crossover(selectParent(), selectParent());

		//make sure the the networks that were selected can be selected in the next loop


		for (IFitness& n : _population) {
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

template <class T>
void GeneticAlgo<T>::mutate(Level & level)
{
	//what ways can we mutate the level
	//TODO: so we need to split up the chromeosome into a sd array of the columns
	//this needs to be a list of the inner columns so that the integrity of the level is maintained

	std::vector<std::string> chromeosome = level.levelToChromeosome();

	std::vector<std::vector<std::string>> columns = std::vector<std::vector<std::string>>();

	//checking the height
	int h = 0;
	bool cscheack = false;
	for (std::string s : chromeosome) {
		if (s == "CS" && cscheack) {
			break;
		}
		else if(s == "CS") {
			cscheack = true;
		}
		else {
			h++;
		}
	}

	int w = 0;
	for (std::string s : chromeosome){
		if (s == "CS") w++; 
	}


	for (int x = 0; x < w; x++) {
		columns.push_back(std::vector<std::string>());
		for (int y = 1; y <= h; y++) {
			columns.back().push_back(chromeosome.at(x*(h + 1) + y));
		}
	}

	//shift up and down, swap columns, invert level, add new mutated level into the mix etc.
	//lets mutate this level here#

	if (Noise::randomFloat(0.0f, 1.0f) >= this->_mutationRate) {
		float random = Noise::randomFloat(0.0f, 1.0f);
		if (random < 0.5) {
			//swap a column with another
			int randA = Noise::randomInt(3, w - 3);
			int randB = Noise::randomInt(3, w - 3);
			std::vector<std::string> temp = columns.at(randA);
			columns.at(randA) = columns.at(randB);
			columns.at(randB) = temp;
		}
		else {
			//convert a colum to a pit
			int randA = Noise::randomInt(3, w - 3);
			std::vector<std::string>& column = columns.at(randA);
			for (int i = 0; i < int(column.size()-1); i++) {
				column.at(i) = "61";
			}
		}
	}


	chromeosome.clear();
	//convert the columns back to a chromeosome
	for (std::vector<std::string> column : columns) {
		chromeosome.push_back("CS");
		for (std::string s : column) {
			chromeosome.push_back(s);
		}
	}

	//convert back to chromeosome then back to the level;

	level.chromeosomeToLevel(chromeosome);
}

template<class T>
CrossoverProduct<T> GeneticAlgo<T>::crossover(Level & A, Level & B)
{
	return CrossoverProduct<T>(A, B);
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
CrossoverProduct<T> GeneticAlgo<T>::crossover(NeuralNetwork & A, NeuralNetwork & B)
{
	std::vector<std::string> chromeosomeA = A.matricesToChromesome();
	std::vector<std::string> chromeosomeB = B.matricesToChromesome();

	int connections = (int)chromeosomeA.size();

	std::vector<std::string> newChromeosomeA = std::vector<std::string>(connections);
	std::vector<std::string> newChromeosomeB = std::vector<std::string>(connections);


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
				float sum = (std::stof(chromeosomeB[i]) + std::stof(chromeosomeA[i])) / 2.0f;
				newChromeosomeA[i] = std::to_string(sum);
				newChromeosomeB[i] = std::to_string(sum);
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

	return CrossoverProduct<T>(NeuralNetwork(A.getTopology(), newChromeosomeA, A.getExtraData()), NeuralNetwork(B.getTopology(), newChromeosomeB, B.getExtraData()));;
}

template <class T>
void GeneticAlgo<T>::saveFittestInPopulation(std::string path, std::string token, std::string subfolder, std::string fileName)
{
	if (typeid(T) == typeid(NeuralNetwork)) {
		NeuralNetwork* network = (NeuralNetwork*)(&fittestInPopulation());
		network->saveNetwork(path, token, subfolder, fileName);
	}
	else if (typeid(T) == typeid(Level)) {
		Level* level = (Level*)(&fittestInPopulation());
		level->writeTileData(path, token, subfolder, fileName);
	}
}

template <class T>
void GeneticAlgo<T>::savePopulation(std::string path, std::string token, std::string subfolder)
{

	if (typeid(T) == typeid(NeuralNetwork)) {
		for (int i = 0; i < _populationSize; i++) {
			NeuralNetwork* network = (NeuralNetwork*)(&this->_population.at(i));
			network->saveNetwork(path, token, subfolder, std::to_string(i));
		}
	}
	else if (typeid(T) == typeid(Level)) {
		for (int i = 0; i < _populationSize; i++) {
			Level* level = (Level*)(&this->_population.at(i));
			level->writeTileData(path, token, subfolder, std::to_string(i));
		}
	}
}

template <class T>
float GeneticAlgo<T>::averageFitness()
{
	float average = 0.0f;
	for (IFitness& a : _population)
	{
		average += a.getFitness();
	}
	average /= _populationSize;
	return average;
}

template <class T>
T& GeneticAlgo<T>::fittestInPopulation()
{
	return _population.front();
}

template <class T>
int GeneticAlgo<T>::numberOfPopAboveFitness(float fitnessmarker)
{
	int count = 0;
	for (IFitness& n : this->_population) {
		if (n.getFitness() >= fitnessmarker) {
			count++;
		}
	}
	return count;
}

template <class T>
void GeneticAlgo<T>::writeGAData(std::string path, std::string token, std::string subfolder)
{
	if (subfolder != "") {
		subfolder = "/" + subfolder;
	}

	std::ofstream csv;
	if (csv.is_open()) {
		csv.close();
	}
	csv.open("Resources/" + path + token + subfolder + "/trainingdata.csv");
	csv << this->_gaData;
	csv.close();
}
