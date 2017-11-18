#pragma once

class InputEvent
{
	friend class InputDevice;

	int _inputID;
	float _inputValue;
	bool _isAxis;
	float _deadzone;

public:
	InputEvent(int id, bool axis, float value, float deadzone);
};

