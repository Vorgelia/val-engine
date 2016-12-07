#pragma once
class InputFrame
{
public:
	unsigned char buttonStates;
	unsigned char axisState;
	//bool used;
	InputFrame inverse();
	InputFrame(unsigned char buttonStates, unsigned char axisState);
	InputFrame();
};

