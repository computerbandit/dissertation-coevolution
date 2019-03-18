#pragma once
#include "../Framework/State.h"
#include "../Framework/Game.h"
#include "../Framework/Level.h"
#include "../Neural/NNControlledPlayer.h"
#include "../Neural/GeneticAlgo.h"
#include <SFML/Graphics.hpp>

class CoEvolveState : virtual public State {
public:
	CoEvolveState(GameDataRef data);
	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;

	virtual void resume() override;
	virtual void pause() override;

	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

private:

	NNControlledPlayer* getBestController();
	void checkProgress(float interval);
	bool areAllDead();

	GameDataRef _data;
	int _currentLevel = 0;
	int _maxGenerations = 0;
	std::vector<std::vector<float>> _tornMatrix;
	

	GeneticAlgo<NeuralNetwork> _networkGA;
	std::vector<NNControlledPlayer> _playerPopulation;

	GeneticAlgo<Level> _levelGA;
	std::vector<Level> _levelPopulation;

	bool _displayTraining = true;
	int ticks = 0;


	sf::Clock _ttlClock;
	sf::Clock _checkProgressClock;
	float _ttl;
	sf::Text _info;
	std::string _token;
};