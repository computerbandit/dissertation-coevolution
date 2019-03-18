#pragma once
#include "IFitness.h"
#include <random>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

enum ActivationFunction {
	SIGMOID, HARDLIM
};

class NeuralNetwork : public IFitness {
private:
	std::vector<Matrix> _layer;
	std::vector<float> _output;
	std::vector<std::string> _extraData;
	int _networkId = 0;

	float sigmoid(float sum, float theta);
	float hardLimit(float sum, float theta);
	
public:

	NeuralNetwork() {}
	NeuralNetwork(std::vector<int> topology); 
	NeuralNetwork(std::string filePath);
	NeuralNetwork(std::vector<int> topology, std::vector<std::string> chromeosome, std::vector<std::string> extraData);
	~NeuralNetwork() {}

	std::string toString() const;
	std::vector<float> matrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function);
	void run(std::vector<float> input);
	const std::vector<float>& getOutput() const;
	void saveNetwork(std::string path, std::string token, std::string subfolder, std::string filename = "") const;
	std::vector<std::string> matricesToChromesome() const;
	std::vector<int> getTopology() const;

	std::vector<Matrix>& getLayers();
	void setLayers(std::vector<Matrix> newLayers);

	const std::vector<std::string>& getExtraData() const;
	void addExtraData(std::vector<std::string> extraData);
	void setExtraData(std::vector<std::string> extraData);

	static std::vector<Matrix> chromeosomeToMatrices(std::vector<int> topology, std::vector<std::string> chromeosome);
	static std::vector<NeuralNetwork> generatePopulation(int populationSize, std::vector<int> topology);
	static float randomFloat(float Min, float Max);
	static int randomInt(int Min, int Max);
	static float randomFloatNromalDist(float mean, float stddev);
	static int newNetworkId();
	static int _networkCount;
};
