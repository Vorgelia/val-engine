#include "InputMotion.h"

InputMotionComponent::InputMotionComponent(std::vector<InputButtonEvent>& buttons, unsigned char direction, int minDuration, int leniency, bool strict)
{
	this->buttons = buttons;
	this->direction = direction;
	this->strict = strict;//Strict makes checking the input only trigger if it's exactly the one specified. Otherwise, down for instance can be triggered with downback, or downforward as well.
	this->minDuration = glm::max(minDuration, 1);
	this->leniency = leniency;
}