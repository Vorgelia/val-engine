#pragma once
class InputEvent
{
public:
	int inputID;
	float inputValue;
	bool isAxis;
	float deadzone;
	InputEvent(int id, bool axis, float value, float deadzone);
};

