#pragma once
#include <vector>

enum ActivationFunction {
	SIGMOID , HARDLIM
};

class Node {
public:
	Node(ActivationFunction functionType, float theta);
	~Node() {}

	void AddInput(float value);
	void ClearInput();
	float Output();

private:

	float SumInputs();

	float SigmoidFunction();
	float HardLimFunction();

	std::vector<float> _inputs;
	ActivationFunction _functionType;
	float _theta;
};

