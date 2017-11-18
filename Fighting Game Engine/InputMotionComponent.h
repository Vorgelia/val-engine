#pragma once
#include <vector>
#include "JSON.h"
#include "InputIdentifiers.h"

typedef std::pair<unsigned char, InputTypeMask> InputButtonEvent;

class InputMotionComponent
{
public:
	std::vector<InputButtonEvent> buttons;
	unsigned char direction;
	bool strict;//If true, direction needs to be matched by inputs exactly
	int leniency;//Maximum amount of frames between this input and previous one
	int minDuration;//Minimum duration for this input.

	InputMotionComponent(std::vector<InputButtonEvent>& buttons, unsigned char direction, int minDuration = 0, int leniency = -1, bool strict = false);
	InputMotionComponent(const json& j);
};
