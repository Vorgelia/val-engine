#include "InputFrame.h"
#include "InputDevice.h"

InputFrame::InputFrame(InputButton buttonStates, InputDirection axisState)
	: _buttonStates(buttonStates)
	, _axisState(axisState)
{
}

InputFrame::InputFrame()
	: _buttonStates(InputButton::None)
	, _axisState(InputDirection::None)
{
}

glm::vec2 InputFrame::ToVector() const
{
	glm::vec2 rv;

	if((_axisState & InputDirection::Left) != InputDirection::None)
		rv.x -= 1;
	if((_axisState & InputDirection::Right) != InputDirection::None)
		rv.x += 1;
	if((_axisState & InputDirection::Up) != InputDirection::None)
		rv.y += 1;
	if((_axisState & InputDirection::Down) != InputDirection::None)
		rv.y -= 1;

	return rv;
}

//Flipped is used for p2 side characters
InputFrame InputFrame::flipped() const
{
	InputFrame rif(this->_buttonStates, this->_axisState);

	if((rif._axisState & (InputDirection::Left | InputDirection::Right)) != InputDirection::None)
		rif._axisState ^= (InputDirection::Left | InputDirection::Right);

	return rif;
}