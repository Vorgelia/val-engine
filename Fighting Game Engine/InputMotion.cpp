#include "InputMotion.h"

InputMotionComponent::InputMotionComponent(std::vector<InputButtonEvent> buttons, unsigned char direction, int minDuration, int leniency, bool strict){
	this->buttons = buttons;
	this->direction = direction;
	this->strict = strict;
	this->minDuration = glm::max(minDuration,1);
	this->leniency = leniency;
}