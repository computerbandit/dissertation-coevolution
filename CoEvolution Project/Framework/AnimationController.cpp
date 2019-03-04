#include "AnimationController.h"

AnimationController::AnimationController(sf::Sprite& sprite) : _spriteRef(sprite)
{

	this->_currentAnimationState = nullptr;
	this->_tpf = 1000.0f;
	this->_frame = 0;
	this->_loop = false;
	this->_change = false;
	this->_flippedx = false;
}

void AnimationController::mapAnimations(AssetManager* am, std::vector<unsigned int> animIds)
{
	//create a map of the related animations for the context
	//for each name given push a pointer to the animstate the asset manager is holding from load in
	for (unsigned int id : animIds) {
		this->_animations[id] = &am->getAnimationState(id);
	}
}

void AnimationController::nextAnimation(unsigned int id, bool loop, bool flippedx)
{
	unsigned int currentID = (this->_currentAnimationState == nullptr) ? 0 : this->_currentAnimationState->_id;
	if (id != currentID) {
		this->_loop = loop;
		this->_currentAnimationState = this->_animations[id];
		this->_tpf = this->_currentAnimationState->_time / this->_currentAnimationState->_length;
		this->_frame = this->_currentAnimationState->_startindex;
		this->_change = true;

		if ((flippedx && !this->_flippedx) || (!flippedx && this->_flippedx)) {
			//flip sprite if the sprite needs to be flipped
			this->_spriteRef.setOrigin(sf::Vector2f(this->_spriteRef.getLocalBounds().width/2, 0.0f));
			this->_spriteRef.scale(sf::Vector2f(-1.0f, 1.0f));
			//this->_spriteRef.setOrigin(sf::Vector2f(0.0f, 0.0f));
			//then offset to ajust for the scale reposition
			if (flippedx) {
				//then offset to ajust for the scale reposition
				
			}
	
			this->_flippedx = flippedx;
		}
		this->_spriteRef.setTexture(this->_currentAnimationState->_sheet->getTexture(this->_frame));
	}
}

void AnimationController::update()
{
	if (this->_currentAnimationState != nullptr) {
		if (this->_change) {
			this->_clock.restart();
			this->_change = false;
		}
		if (!this->_currentAnimationState->_static && 
			this->_clock.getElapsedTime().asMilliseconds() >= this->_tpf) {

			if (this->_loop) {
				this->_frame = ((this->_frame + 1) % this->_currentAnimationState->_length) + this->_currentAnimationState->_startindex;
			}
			else {
				if (this->_frame + 1 < (this->_currentAnimationState->_startindex + (this->_currentAnimationState->_length - 1))) {
					this->_frame++;
					this->_timeout = false;
				}
				else {
					this->_timeout = true;
				}
			}
			this->_spriteRef.setTexture(this->_currentAnimationState->_sheet->getTexture(this->_frame));
			//restart clock
			this->_clock.restart();
		}
	}
}


