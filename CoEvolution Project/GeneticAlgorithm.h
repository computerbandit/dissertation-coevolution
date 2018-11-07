#pragma once
#include <vector>
#include "Chromeosome.h"


template <class T>
class GeneticAlgorithm {
public:
	GeneticAlgorithm() {}
	~GeneticAlgorithm() {}

	//

private:
	std::vector<Chromeosome<T>> _population;
	int _generation = 0;

};