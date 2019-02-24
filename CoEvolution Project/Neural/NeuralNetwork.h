#pragma once
#include <random>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

enum ActivationFunction {
	SIGMOID, HARDLIM
};

static int _networkCount = 0;

class NeuralNetwork {
private:
	std::vector<Matrix> _layer;
	std::vector<float> _output;
	float _fitnessScore = 0.0f, _fitnessRatio = 0.0f;
	bool _selected = false;
	std::vector<float> _chromeosome;

	float sigmoid(float sum, float theta);
	float hardLimit(float sum, float theta);
	
public:

	NeuralNetwork() {}
	NeuralNetwork(std::vector<int> topology); 
	NeuralNetwork(std::string filePath);
	NeuralNetwork(std::vector<int> topology, std::vector<float> chromeosome);
	~NeuralNetwork() {}

	std::string toString() const;
	std::vector<float> matrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function);
	void run(std::vector<float> input);
	const std::vector<float>& getOutput() const;
	void saveNetwork(std::string token) const;
	std::vector<float> matricesToChromesome() const;
	std::vector<int> getTopology() const;

	const float& getFitnessScore() const;
	void setFitnessScore(float fitnessScore);

	const float& getFitnessRatio() const;
	void setFitnessRatio(float fitnessRatio);

	std::vector<Matrix>& getLayers();
	void setLayers(std::vector<Matrix> newLayers);

	const bool& isSelected() const;
	void setSelected(bool selected);

	const std::vector<float>& getChromeosome() const;

	static std::vector<Matrix> chromeosomeToMatrices(std::vector<int> topology, std::vector<float> chromeosome);
	static std::vector<NeuralNetwork> generatePopulation(int populationSize, std::vector<int> topology);
	static float randomFloat(float Min, float Max);
	static int randomInt(int Min, int Max);
	static float randomFloatNromalDist(float mean, float stddev);
	static int newNetworkId();
};