#pragma once
#include <random>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

enum ActivationFunction {
	SIGMOID, HARDLIM
};

static int _networkCount = 0;

class NeuralNetwork {

	std::vector<Matrix> _layer;
	std::vector<float> _output;
	float _fitnessScore = 0.0f, _fitnessRatio = 0.0f;
	bool selected = false;

	float Sigmoid(float sum, float theta);
	float HardLimit(float sum, float theta);
	
public:

	NeuralNetwork() {}
	NeuralNetwork(std::vector<int> topology);
	NeuralNetwork(std::vector<Matrix> layers);
	~NeuralNetwork() {}

	std::string ToString() const;
	std::vector<float> MatrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function);
	void Run(std::vector<float> input);
	const std::vector<float>& GetOutput() const;
	void SaveNetwork(std::string filePath = "") const;
	std::vector<float> MatricesToChromesome() const;
	std::vector<int> GetTopology() const;

	const float& GetFitnessScore() const;
	void SetFitnessScore(float fitnessScore);

	const float& GetFitnessRatio() const;
	void SetFitnessRatio(float fitnessRatio);

	const std::vector<Matrix>& GetLayers() const;
	void SetLayers(std::vector<Matrix> newLayers);

	const bool& Selected() const;
	void ToggleSelected();

	static std::vector<NeuralNetwork> GeneratePopulation(int populationSize, std::vector<int> topology);
	static float RandomFloat(float Min, float Max);
	static int RandomInt(int Min, int Max);

	static float RandomFloatNromalDist(float mean, float stddev);

	static int NewNetworkId();
};