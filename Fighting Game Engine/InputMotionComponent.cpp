#include "InputMotionComponent.h"
#include <GLM/glm.hpp>

InputMotionComponent::InputMotionComponent(std::vector<InputButtonEvent>& buttons, InputDirection direction, int minDuration, int leniency, bool strict)
{
	this->buttons = buttons;
	this->direction = direction;
	this->strict = strict;
	this->minDuration = glm::max(minDuration, 1);
	this->leniency = leniency;
}

InputMotionComponent::InputMotionComponent(const json& j)
{
	for(auto& iter : j["buttons"])
	{
		buttons.emplace_back(
			InputButton(JSON::Get<unsigned char>(iter["button"])),
			InputTypeMask(JSON::Get<unsigned char>(iter["type"]))
		);
	}

	direction = InputDirection(JSON::Get<unsigned char>(j["direction"]));
	minDuration = JSON::Get<int>(j["minDuration"]);
	leniency = JSON::Get<int>(j["leniency"]);
	strict = JSON::Get<bool>(j["strict"]);
}