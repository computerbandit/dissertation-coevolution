#include "Node.h"
#include <cmath>

Node::Node(ActivationFunction functionType, float theta): _functionType(functionType), _theta(theta)
{
}

void Node::AddInput(float value)
{
	this->_inputs.push_back(value);
}

void Node::ClearInput()
{
	this->_inputs.clear();
}

float Node::Output()
{
	switch (_functionType)
	{
	case SIGMOID:
		return this->SigmoidFunction();
		break;
	case HARDLIM:
		return this->HardLimFunction();
		break;
	default:
		return this->HardLimFunction();
		break;
	}
	this->ClearInput();
}

void Node::SetIn(NodeConnection * in)
{
	_in = in;
}

void Node::SetOut(NodeConnection * out)
{
	_out = out;
}

float Node::SumInputs()
{
	float sum = 0.0f;
	for (float in : _inputs) { sum += in; }
	return 	sum;
}

float Node::SigmoidFunction()
{
	return (1 / (1 + (std::exp(-(SumInputs() - _theta)))));
}

float Node::HardLimFunction()
{

	if (SumInputs() >= _theta) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}
