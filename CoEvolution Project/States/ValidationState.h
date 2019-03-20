#pragma once
#include "../Framework/State.h"
#include "../Framework/Game.h"
#include "../Framework/Level.h"
#include "../Neural/NNControlledPlayer.h"
#include <SFML/Graphics.hpp>

class ValidationState : public State {
public:
	ValidationState(GameDataRef data, std::string token, int popSize);
	// Inherited via State
	virtual void init() override;
	virtual void cleanup() override;
	virtual void handleEvents() override;
	virtual void update(float dt) override;
	virtual void draw(float dt) override;

private:
	GameDataRef _data;
	std::vector<Level> _levels;
	int _currentLevel = 0;
	int _currentNetwork = 0;
	int _populationSize = 0;
	std::string _token = "";
	std::vector<NNControlledPlayer> _population;
	std::vector<std::vector<float>> _tornMatrix;

	std::string _valiData = "";

	sf::Clock _ttlClock;
	sf::Clock _checkProgressClock;
	float _ttl;

	void saveValiData(std::string path, std::string token, std::string subfolder);


	bool areAllDead();
	

};