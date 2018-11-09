#pragma once
#include <vector>
#include "Chromeosome.h"

template <class T>
using Population = std::vector<Chromeosome<T>>;

template <class T>
struct CrossoverProduct
{
	Chromeosome<T> _childA;
	Chromeosome<T> _childB;
};

template <class T>
class GeneticAlgorithm {
public:
	GeneticAlgorithm(Population<T> initPopulation, float crossoverRate, float mutationRate);
	~GeneticAlgorithm() {}

	void Evaluate();
	void Selection();
	CrossoverProduct<T> Crossover(Chromeosome<T> A, Chromeosome<T> B);
	void Mutation(Chromeosome<T> chromeosome);

	Population<T>& GetPopulation() {
		return _population;
	}
private:
	Population<T> _population;
	int _generation;
	float _crossoverRate, _mutationRate;
};

template<class T>
inline GeneticAlgorithm<T>::GeneticAlgorithm(Population<T> initPopulation, float crossoverRate, float mutationRate) : _population(initPopulation), _crossoverRate(crossoverRate), _mutationRate(mutationRate), _generation(0)
{
}

//this function will evaluate the current population and sort the chromeosomes into order of fitness
template<class T>
inline void GeneticAlgorithm<T>::Evaluate()
{

}

template<class T>
inline void GeneticAlgorithm<T>::Selection()
{


}
;