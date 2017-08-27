#include "InputEvent.h"

InputEvent::InputEvent(int id, bool axis, float value, float deadzone)
{
	this->inputID = id;
	this->isAxis = axis;
	this->inputValue = value;
	this->deadzone = deadzone;
}

