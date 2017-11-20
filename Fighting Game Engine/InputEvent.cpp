#include "InputEvent.h"

InputEvent::InputEvent(int id, bool axis, float value, float deadzone)
{
	this->_inputID = id;
	this->_isAxis = axis;
	this->_inputValue = value;
	this->_deadzone = deadzone;
}

