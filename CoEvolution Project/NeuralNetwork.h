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

	float Sigmoid(float sum, float theta);
	float HardLimit(float sum, float theta);
	
public:

	NeuralNetwork() {}
	NeuralNetwork(std::vector<int> topology); 
	NeuralNetwork(std::string filePath);
	NeuralNetwork(std::vector<int> topology, std::vector<float> chromeosome);
	~NeuralNetwork() {}

	std::string ToString() const;
	std::vector<float> MatrixOutput(const Matrix& m, std::vector<float> input, ActivationFunction function);
	void Run(std::vector<float> input);
	const std::vector<float>& GetOutput() const;
	void SaveNetwork(std::string token) const;
	std::vector<float> MatricesToChromesome() const;
	std::vector<int> GetTopology() const;

	const float& GetFitnessScore() const;
	void SetFitnessScore(float fitnessScore);

	const float& GetFitnessRatio() const;
	void SetFitnessRatio(float fitnessRatio);

	std::vector<Matrix>& GetLayers();
	void SetLayers(std::vector<Matrix> newLayers);

	const bool& IsSelected() const;
	void SetSelected(bool selected);

	const float& GetMutationRate() const;
	void SetMutationRate(float rate);

	const std::vector<float>& GetChromeosome() const;

	static std::vector<Matrix> ChromeosomeToMatrices(std::vector<int> topology, std::vector<float> chromeosome);
	static std::vector<NeuralNetwork> GeneratePopulation(int populationSize, std::vector<int> topology);
	static float RandomFloat(float Min, float Max);
	static int RandomInt(int Min, int Max);
	static float RandomFloatNromalDist(float mean, float stddev);
	static int NewNetworkId();
};