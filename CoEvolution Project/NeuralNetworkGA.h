#pragma once
#include <vector>
#include <map>
#include "NeuralNetwork.h"

struct CrossoverProduct
{
	NeuralNetwork A, B;
	CrossoverProduct(NeuralNetwork a, NeuralNetwork b) :A(a), B(b) {};
};

class NeuralNetworkGA{

	std::vector<NeuralNetwork> _population;
	int _populationSize = 0, _generation = 0;
	float _mutationRate = 0.05f;
	int MAX_GENERATIONS = 10;
	bool _solution = false;

public:

	NeuralNetworkGA() {}
	NeuralNetworkGA(std::vector<NeuralNetwork> population, float mRate);
	~NeuralNetworkGA() {}
	
	void Run(std::vector<float> input);
	std::vector<NeuralNetwork>& GetPopulation();

	const int& GetGeneration()const;

	void Solved();
	const bool& isSolved() const;

	NeuralNetwork& SelectParent();
	void EvalutePopulation();
	void NextGeneration();
	void Mutate(NeuralNetwork& network);
	CrossoverProduct Crossover(NeuralNetwork& A, NeuralNetwork& B);
	void SaveFittestNetwork();
	float AverageFitness();
	const NeuralNetwork& FittestNetwork();
};