#pragma once
#include <vector>
class ConnectionWeight;

enum ActivationFunction {
	SIGMOID , HARDLIM
};

class Node {
public:
	Node(ActivationFunction functionType, float theta);
	~Node() {}

	void GenOutput();

	void SetRawInput(float input) {
		this->_rawInput = input;
	}

	const float& Output() {
		return _output;
	}

	void AssignInput(ConnectionWeight* ipnut);

private:

	float SumInputs();
	float SigmoidFunction();
	float HardLimFunction();

	std::vector<ConnectionWeight*> _inputs;
	ActivationFunction _functionType;
	float _theta, _output, _rawInput;
};

