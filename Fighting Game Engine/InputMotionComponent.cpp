#include "InputMotionComponent.h"
#include <GLM\glm.hpp>

InputMotionComponent::InputMotionComponent(std::vector<InputButtonEvent>& buttons, unsigned char direction, int minDuration, int leniency, bool strict)
{
	this->buttons = buttons;
	this->direction = direction;
	this->strict = strict;//Strict makes checking the input only trigger if it's exactly the one specified. Otherwise, down for instance can be triggered with downback, or downforward as well.
	this->minDuration = glm::max(minDuration, 1);
	this->leniency = leniency;
}

InputMotionComponent::InputMotionComponent(const json& j)
{
	for(auto& iter : j["buttons"])
	{
		buttons.push_back(
		std::make_pair(
			JSON::Get<unsigned char>(iter["button"]),
			(InputTypeMask)JSON::Get<unsigned char>(iter["type"])
		));
	}

	direction = JSON::Get<unsigned char>(j["direction"]);
	minDuration = JSON::Get<int>(j["minDuration"]);
	leniency = JSON::Get<int>(j["leniency"]);
	strict = JSON::Get<bool>(j["strict"]);
}