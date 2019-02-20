#pragma once
#include <vector>
#include <map>
#include "NeuralNetwork.h"

//comment
struct CrossoverProduct
{
	NeuralNetwork A, B;
	CrossoverProduct(NeuralNetwork a, NeuralNetwork b) :A(a), B(b) {};
};

class NeuralNetworkGA{

	std::vector<NeuralNetwork> _population;
	int _populationSize = 0, _generation = 0;
	float _mutationRate = 0.999f;
	bool _solution = false;
	std::string _gaData = "";

public:

	NeuralNetworkGA() {}
	NeuralNetworkGA(std::vector<NeuralNetwork> population, float mRate);
	~NeuralNetworkGA() {}
	
	void run(std::vector<float> input);
	std::vector<NeuralNetwork>& getPopulation();

	const int& getGeneration()const;

	void solved();
	const bool& isSolved() const;

	void setMutationRate(float mRate);
	const float& getMutationRate() const;

	NeuralNetwork& selectParent();
	void evalutePopulation();
	float sumPopulationScores();
	void setPopulationFitnessRatios(float sum);
	void sortPopulation();


	void nextGeneration();
	void mutate(NeuralNetwork& network);
	CrossoverProduct crossover(NeuralNetwork& A, NeuralNetwork& B);
	void saveFittestNetwork(std::string token);
	float averageFitness();
	const NeuralNetwork& fittestNetwork();


	int numberOfNNAboveFitness(float fitnessmarker);
	void saveGAData(std::string token);
};