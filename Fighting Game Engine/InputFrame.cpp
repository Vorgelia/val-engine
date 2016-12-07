#include "InputFrame.h"
#include "InputDevice.h"

InputFrame::InputFrame(unsigned char buttonStates, unsigned char axisState){
	this->buttonStates=buttonStates;
	this->axisState = axisState;
	//used = false;
}
InputFrame::InputFrame(){
	this->buttonStates =0;
	this->axisState = 0;
	//used = false;
}
InputFrame InputFrame::inverse(){
	InputFrame rif(this->buttonStates,this->axisState);
	if ((rif.axisState&((int)InputDirection::Left | (int)InputDirection::Right)) > 0)
		rif.axisState ^= ((int)InputDirection::Left | (int)InputDirection::Right);
	return rif;
}