#pragma once
#include <vector>

//Bitmask, even though it might not look like it.
enum class InputType
{
	Pressed = 1,
	Released = 2,
	Event = 3,
	Held = 4
};

typedef std::pair<unsigned char, InputType> InputButtonEvent;

class InputMotionComponent
{
public:
	std::vector<InputButtonEvent> buttons;
	unsigned char direction;
	bool strict;//If true, direction needs to be matched by inputs exactly
	int leniency;//Maximum amount of frames between this input and previous one
	int minDuration;//Minimum duration for this input.

	InputMotionComponent(std::vector<InputButtonEvent>& buttons, unsigned char direction, int minDuration = 0, int leniency = -1, bool strict = false);
};
typedef std::vector<InputMotionComponent> InputMotion;