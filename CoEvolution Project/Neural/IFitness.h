#pragma once
#include <vector>

class IFitness {
public:
	~IFitness() {}

	inline const std::vector<std::string>& getChromeosome() const { return _chromeosome; }
	inline void setChromeosome(std::vector<std::string> chromeosome) { _chromeosome = chromeosome; }

	inline const float& getFitness() const { return _fitness; }
	inline void setFitness(float fitness) { _fitness = fitness; }

	inline const float& getFitnessRatio() const { return _fitnessRatio; }
	inline void setFitnessRatio(float fitnessRatio) { _fitnessRatio = fitnessRatio; }

	inline const bool & isSelected() const { return _selected; }
	inline void setSelected(bool selected) { _selected = selected; }

protected:
	float _fitness = 0.0f, _fitnessRatio = 0.0f;
	bool _selected = false;
	std::vector<std::string> _chromeosome;
};