#pragma once
#include<GLM/glm.hpp>

class InputFrame
{
	friend class InputDevice;

	unsigned char _buttonStates;
	unsigned char _axisState;

public:
	unsigned char buttonStates() const;
	unsigned char axisState() const;

	InputFrame flipped() const;
	glm::vec2 ToVector() const;

	InputFrame(unsigned char buttonStates, unsigned char axisState);
	InputFrame();
};

