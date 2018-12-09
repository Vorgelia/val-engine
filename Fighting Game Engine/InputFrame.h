#pragma once
#include<GLM/glm.hpp>
#include "InputIdentifiers.h"

class InputFrame
{
	friend class InputDevice;

	InputButton _buttonStates;
	InputDirection _axisState;

public:
	InputButton buttonStates() const { return _buttonStates; }
	InputDirection axisState() const { return _axisState; }

	InputFrame flipped() const;
	glm::vec2 ToVector() const;

	InputFrame(InputButton buttonStates, InputDirection axisState);
	InputFrame();
};

