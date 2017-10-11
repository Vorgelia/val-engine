#include "InputFrame.h"
#include "InputDevice.h"

InputFrame::InputFrame(unsigned char buttonStates, unsigned char axisState)
{
	this->buttonStates = buttonStates;
	this->axisState = axisState;
}
InputFrame::InputFrame()
{
	this->buttonStates = 0;
	this->axisState = 0;
}
glm::vec2 InputFrame::ToVector()
{
	glm::vec2 rv;

	if((axisState & (unsigned char)InputDirection::Left) != 0)
		rv.x -= 1;
	if((axisState & (unsigned char)InputDirection::Right) != 0)
		rv.x += 1;
	if((axisState & (unsigned char)InputDirection::Up) != 0)
		rv.y += 1;
	if((axisState & (unsigned char)InputDirection::Down) != 0)
		rv.y -= 1;

	return rv;
}
//Flipped is used for p2 side characters
InputFrame InputFrame::flipped()
{
	InputFrame rif(this->buttonStates, this->axisState);

	if((rif.axisState&((unsigned char)InputDirection::Left | (unsigned char)InputDirection::Right)) > 0)
		rif.axisState ^= ((unsigned char)InputDirection::Left | (unsigned char)InputDirection::Right);

	return rif;
}