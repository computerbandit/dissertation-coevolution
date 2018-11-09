#pragma once
#include <vector>

template <class T>
class Chromeosome {
public:
	Chromeosome() {}
	Chromeosome(std::vector<T> sequence);
	~Chromeosome() {}

	bool operator() (const Chromeosome<T>& A, const Chromeosome<T>& B);

	void SetFitness(float fitness) {
		this->_fitness = fitness;
	}
	void SetFitnessRatio(float fitnessRatio) {
		this->_fitnessRatio = fitnessRatio;
	}
	float GetFitness() {
		return this->_fitness;
	}
	float GetFitnessRatio(float fitnessRatio) {
		return this->_fitnessRatio;
	}
	
	std::vector<T>& GetSequence() {
		return _sequence;
	}
private:
	std::vector<T> _sequence;
	float _fitness, _fitnessRatio;
};

template<class T>
inline Chromeosome<T>::Chromeosome(std::vector<T> sequence):_sequence(sequence)
{
	this->_fitness = 0.0f;
	this->_fitnessRatio = 0.0f;
}

template<class T>
inline bool Chromeosome<T>::operator()(const Chromeosome<T>& A, const Chromeosome<T>& B)
{
	return (A._fitness > B._fitness);
}
