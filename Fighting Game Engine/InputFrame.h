#pragma once
#include<GLM\glm.hpp>
class InputFrame
{
public:
	unsigned char buttonStates;
	unsigned char axisState;
	
	InputFrame inverse();
	glm::vec2 ToVector();
	InputFrame(unsigned char buttonStates, unsigned char axisState);
	InputFrame();
};

