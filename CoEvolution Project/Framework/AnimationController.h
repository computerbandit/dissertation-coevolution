#pragma once
#include <SFML/Graphics.hpp>
#include "Texturesheet.h"
#include "AssetManager.h"
class AssetManager;

struct AnimationState {
	std::string _name;
	Texturesheet* _sheet;
	int _startindex = 0;
	int _length = 1;
	float _time = 1000.0f;
	bool _static = true;
	AnimationState() {}
	AnimationState(std::string name , Texturesheet* sheet, int startindex, int length, float time, bool single) : _name(name), _sheet(sheet), _startindex(startindex), _length(length), _time(time), _static(single){}
};

class AnimationController {
public:
	AnimationController(sf::Sprite& sprite);
	virtual ~AnimationController() {}

	void mapAnimations(AssetManager* am , std::vector<std::string> animNames);
	void nextAnimation(std::string name, bool loop);

	void update();
private:
	sf::Sprite& _spriteRef;
	std::map<std::string, AnimationState*> _animations;
	AnimationState* _currentAnimationState;
	sf::Clock _clock;
	float _tpf;
	int _frame;
	bool _loop;
	bool _change;
};
