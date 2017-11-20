#include "InputFrame.h"
#include "InputDevice.h"

InputFrame::InputFrame(unsigned char buttonStates, unsigned char axisState)
{
	this->_buttonStates = buttonStates;
	this->_axisState = axisState;
}

InputFrame::InputFrame()
{
	this->_buttonStates = 0;
	this->_axisState = 0;
}

glm::vec2 InputFrame::ToVector() const
{
	glm::vec2 rv;

	if((_axisState & (unsigned char)InputDirection::Left) != 0)
		rv.x -= 1;
	if((_axisState & (unsigned char)InputDirection::Right) != 0)
		rv.x += 1;
	if((_axisState & (unsigned char)InputDirection::Up) != 0)
		rv.y += 1;
	if((_axisState & (unsigned char)InputDirection::Down) != 0)
		rv.y -= 1;

	return rv;
}

unsigned char InputFrame::buttonStates() const
{
	return _buttonStates;
}

unsigned char InputFrame::axisState() const
{
	return _axisState;
}

//Flipped is used for p2 side characters
InputFrame InputFrame::flipped() const
{
	InputFrame rif(this->_buttonStates, this->_axisState);

	if((rif._axisState & ((unsigned char)InputDirection::Left | (unsigned char)InputDirection::Right)) != 0)
		rif._axisState ^= ((unsigned char)InputDirection::Left | (unsigned char)InputDirection::Right);

	return rif;
}