#pragma once
#include <vector>
class NodeConnection;

enum ActivationFunction {
	SIGMOID , HARDLIM
};

class Node {
public:
	Node(ActivationFunction functionType, float theta);
	~Node() {}

	void AddInput(float value);
	float Output();

	void SetIn(NodeConnection *in);
	void SetOut(NodeConnection *out);

	NodeConnection * GetOutgoing() {
		return _out;
	}
	NodeConnection * GetIncoming() {
		return _in;
	}

private:
	void ClearInput();
	float SumInputs();
	float SigmoidFunction();
	float HardLimFunction();

	NodeConnection *_in, *_out;

	std::vector<float> _inputs;
	ActivationFunction _functionType;
	float _theta;
};

