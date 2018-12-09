#include "InputEvent.h"

InputEvent::InputEvent(int id, bool isAxis, float inputValue, float deadzone)
	: inputID(id)
	, inputValue(inputValue)
	, isAxis(isAxis)
	, deadzone(deadzone)
{
}
