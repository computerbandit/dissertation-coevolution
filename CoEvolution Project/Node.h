#pragma once
#include <vector>

class Node {
public:
	Node(float activationFunction);
	~Node() {}

	void ProcessNode();

	void AddInput(float value);
	float GetOutput();

	float& GetActivationFunction() {
		return _activationFunction;
	}

private:
	float _activationFunction;
	std::vector<float> _inputs;
};

