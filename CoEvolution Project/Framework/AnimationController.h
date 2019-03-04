#pragma once
#include <SFML/Graphics.hpp>
#include "Texturesheet.h"
#include "AssetManager.h"
class AssetManager;

struct AnimationState {
	unsigned int _id = 0;
	Texturesheet* _sheet = nullptr;
	int _startindex = 0;
	int _length = 1;
	float _time = 1000.0f;
	bool _static = true;
	AnimationState() {}
	AnimationState(unsigned int id, Texturesheet* sheet, int startindex, int length, float time, bool single) : _id(id), _sheet(sheet), _startindex(startindex), _length(length), _time(time), _static(single){}
};

class AnimationController {
public:
	AnimationController(sf::Sprite& sprite);
	virtual ~AnimationController() {}

	void mapAnimations(AssetManager* am , std::vector<unsigned int> animIds);
	void nextAnimation(unsigned int id, bool loop, bool flippedx);

	inline bool& hasCurrentAnimTimedOut() { return _timeout; }

	void update();
private:
	sf::Sprite& _spriteRef;
	std::map<unsigned int, AnimationState*> _animations;
	AnimationState* _currentAnimationState;
	sf::Clock _clock;
	float _tpf;
	int _frame;
	bool _loop;
	bool _flippedx;
	bool _change;
	bool _timeout;
};
