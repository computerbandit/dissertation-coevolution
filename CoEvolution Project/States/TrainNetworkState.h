#pragma once
#include "../Framework/State.h"
#include "../Framework/Game.h"
#include "../Framework/Level.h"
#include "../Neural/NNControlledPlayer.h"
#include "../Neural/GeneticAlgo.h"
#include <SFML/Graphics.hpp>

class TrainNetworkState : public State {
public:
	TrainNetworkState(GameDataRef data);
	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;

	virtual void resume() override;
	virtual void pause() override;

	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

private:
	bool nextPopulationChunk();
	NNControlledPlayer* getBestController();
	void checkProgress(float interval);
	bool areAllDead();
	void selectLevelForChunk();


	GameDataRef _data;
	std::vector<Level> _levels;
	int _currentLevel = 0;

	GeneticAlgo<NeuralNetwork> _ga;
	std::vector<NNControlledPlayer> _playerPopulation;
	std::vector<NNControlledPlayer*> _populationChunk;
	int _chunkSize = 10000;
	int _chunkIndex = 0;
	bool _lastChunk = false;
	bool _displayTraining = true;
	int ticks = 0;

	sf::Clock _ttlClock;
	sf::Clock _checkProgressClock;
	float _ttl;
	sf::Text _info;
	std::string _token;
};