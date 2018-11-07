#include "Node.h"
#include "ConnectionWeight.h"
#include <cmath>

Node::Node(ActivationFunction functionType, float theta): _functionType(functionType), _theta(theta)
{
	this->_rawInput = 0.0f;
	this->_inputs = std::vector<ConnectionWeight*>();
	this->_output = 0.0f;
}

void Node::AssignInput(ConnectionWeight * input)
{
	this->_inputs.push_back(input);
}


void Node::GenOutput()
{
	float out = 0.0f;
	switch (_functionType)
	{
	case SIGMOID:
		out = this->SigmoidFunction();
		break;
	case HARDLIM:
		out = this->HardLimFunction();
		break;
	default:
		out = this->HardLimFunction();
		break;
	}
	this->_output = out;
}


float Node::SumInputs()
{
	float sum = 0.0f;

	if (!_inputs.empty()) {
		for (ConnectionWeight * input : _inputs) {
			sum += input->GetWeight() * input->GetPrevNode().Output();
		}
	}
	else {
		sum = this->_rawInput;
	}

	return sum;
}

float Node::SigmoidFunction()
{
	float sum = SumInputs();
	return (std::exp(sum - _theta) - std::exp(-(sum - _theta))) / (std::exp(sum - _theta) + std::exp(-(sum - _theta)));
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
