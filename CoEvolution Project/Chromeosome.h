#pragma once
#include <vector>

template <class T>
class Chromeosome {
public:
	Chromeosome(std::vector<T> sequence);
	~Chromeosome() {}


	//mutate the sequence

private:
	std::vector<T> _sequence;
	float _fitness;
};

template<class T>
inline Chromeosome<T>::Chromeosome(std::vector<T> sequence):_sequence(sequence)
{
	this->_fitness = 0;
}
