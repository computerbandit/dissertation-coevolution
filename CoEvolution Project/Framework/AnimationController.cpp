#include "AnimationController.h"

AnimationController::AnimationController(sf::Sprite& sprite) : _spriteRef(sprite)
{

	this->_currentAnimationState = nullptr;
	this->_tpf = 1000.0f;
	this->_frame = 0;
	this->_loop = false;
	this->_change = false;
}

void AnimationController::mapAnimations(AssetManager* am, std::vector<std::string> animNames)
{
	//create a map of the related animations for the context
	//for each name given push a pointer to the animstate the asset manager is holding from load in
	for (std::string name : animNames) {
		this->_animations[name] = &am->getAnimationState(name);
	}
}

void AnimationController::nextAnimation(std::string name, bool loop)
{
	std::string currentName = (this->_currentAnimationState == nullptr) ? "" : this->_currentAnimationState->_name;
	if (name.compare(currentName)) {
		this->_loop = loop;
		this->_currentAnimationState = this->_animations[name];
		this->_tpf = this->_currentAnimationState->_time / this->_currentAnimationState->_length;
		this->_frame = this->_currentAnimationState->_startindex;
		this->_change = true;

		this->_spriteRef.setTexture(this->_currentAnimationState->_sheet->getTexture(this->_frame));

	}
}

void AnimationController::update()
{
	if (this->_change) {
		this->_clock.restart();
		this->_change = false;
	}
	
	if (this->_clock.getElapsedTime().asMilliseconds() >= this->_tpf) {

		if (this->_loop) {
			this->_frame = (this->_frame + 1) % this->_currentAnimationState->_length;
		}
		else {
			if (this->_frame + 1 >= (this->_currentAnimationState->_startindex + (this->_currentAnimationState->_length - 1))) {
				this->_frame++;
			}
		}
		this->_spriteRef.setTexture(this->_currentAnimationState->_sheet->getTexture(this->_frame));
		//restart clock
		this->_clock.restart();
	}

}


