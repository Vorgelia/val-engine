#pragma once

struct InputEvent
{
	int inputID;
	float inputValue;
	bool isAxis;
	float deadzone;

	InputEvent(int id, bool isAxis, float inputValue, float deadzone);
};