#include "InputDevice.h"
#include "ResourceLoader.h"
#include "Time.h"
#include "Screen.h"
#include "InputMotion.h"

//1200 keeps track of the last 20 seconds of inputs, but it can easily be increased if that's not enough
#define VE_INPUT_BUFFER_SIZE 1200

InputDevice::InputDevice(int deviceID)
{
	this->_deviceID = deviceID;
	//Device IDs map to GLFW device IDs, with two extra ones. -1 for keyboard and -2 for inactive
	if(deviceID == -2)
		this->_deviceName = std::string("Inactive");
	else
	{
		if(deviceID == -1)
			this->_deviceName = std::string("Keyboard");
		else
			this->_deviceName = std::string(glfwGetJoystickName(deviceID));

		this->_deviceFilename = this->_deviceName;

		for(char& c : this->_deviceFilename)
		{
			if(std::string::npos != std::string("\\/:?\"<>|").find(c))
				c = '_';
		}

		ResourceLoader::LoadControlSettings("Settings/Input/" + this->_deviceFilename + ".vi", &(this->directionMap), &(this->buttonMap));
	}
	inputBuffer = new InputBuffer(VE_INPUT_BUFFER_SIZE);

}
InputDevice::~InputDevice()
{
	delete inputBuffer;
}
//Helper function for evaluating whether a specific input event is occuring.
//Needed to abstract checking for buttons and axes to a single function with boolean output.
bool InputDevice::EvaluateInput(InputEvent& ie)
{
	if(this->_deviceID == -2)
	{
		return false;
	}
	else if(this->_deviceID == -1)
	{
		return glfwGetKey(Screen::window, ie.inputID) == GLFW_PRESS;
	}
	else
	{
		if(!ie.isAxis)
		{
			return cachedJoyButtons[ie.inputID] > 0;
		}
		else
		{
			return (glm::abs(cachedJoyAxes[ie.inputID]) > ie.deadzone) && (glm::sign(cachedJoyAxes[ie.inputID]) == glm::sign(ie.inputValue));
		}
	}
}
//Default input leniency. We want the first input of every move to only count if it's the latest one by default.
#define INPUT_BUFFER_INIT 1
#define INPUT_BUFFER_MID 8
std::string debugString;
//This function is fairly simple. It checks if a specific motion has been performed by validating every part of the motion based on the distance of its beginning.
//For a quarter circle forward L, it would check if L has been pressed, then the distance between L and a forward input, then the distance between the forward input to the down-forward input, etc
//The distance checking is abstracted in the InputMotionDistance function.
bool InputDevice::EvaluateMotion(InputMotion& motion, bool inverse)
{
	if(motion.size() == 0)
		return false;
	int buf = motion.back().leniency > -1 ? motion.back().leniency : INPUT_BUFFER_INIT;
	int bufferIndex = inputBuffer->position() - 1;
	debugString = "";
	for(int i = motion.size() - 1; i >= 0;)
	{
		debugString += "Checking frame: " + std::to_string(bufferIndex) + "\n";
		debugString += "-Requirements: " + std::to_string((int)motion[i].direction) + ":";
		for(unsigned int bti = 0; bti < motion[i].buttons.size(); ++bti)
		{
			debugString += std::to_string((int)motion[i].buttons[0].first) + ",";
		}
		debugString += '\n';
		int distance = InputMotionDistance(bufferIndex, motion[i], buf, i == 0);
		if(distance >= 0 && distance <= buf)
		{
			debugString += "-Succeeded. Distance=" + std::to_string(distance) + "\n";
			bufferIndex -= distance + motion[i].minDuration - 1;
			--i;
			if(i <= -1)
			{
				//std::cout << "MotionDebug: Frame " << Time::frameCount << std::endl << debugString << "--SUCCESS--\n--------" << std::endl;
				return true;
			}
			else
				buf = motion[i].leniency > -1 ? motion[i].leniency : INPUT_BUFFER_MID;
		}
		else /*if(buf<=-1)*/
		{
			//if (i != motion.size() - 1)
			//std::cout << "MotionDebug: Frame " << Time::frameCount << std::endl << debugString << "--FAILED--\n--------" << std::endl;
			return false;
		}
		/*else{
			//debugString += "-Failed\n";
			//bufferIndex -= 1;
			return false;
			}*/
	}
	//std::cout << "MotionDebug: " << Time::frameCount << std::endl << debugString << "--FAILED OUTSIDE--\n--------" << std::endl;
	return false;
}
//This function checks the distance from an input's beginning by checking back in the buffer until the specified input exceeds its necessary duration and stops being valid.
//For instance, holding forward in a quarter circle forward for 4 frames means the input is valid and turns invalid in a 10f window, and therefore it's valid.
//Max buffer is used as an early out to prevent from checking too far if we're going to discard based on distance being too high anyway
int InputDevice::InputMotionDistance(int currentIndex, InputMotionComponent& motionComp, int maxBuffer, bool firstInput)
{
	int cind = currentIndex;
	int duration = 0;
	do
	{
		//Look back into the buffer and check if motionComp is valid for that frame. If it is, increase the duration of the input.
		//If the duration is over the needed duration, keep checking to see when the input started
		if(InputMotionFrameCheck(motionComp, cind))
		{
			debugString += "--Valid Frame " + std::to_string(cind) + " dur:" + std::to_string(duration + 1) + "|" + std::to_string((int)inputBuffer->at(cind)->axisState) + "," + std::to_string((int)inputBuffer->at(cind)->buttonStates) + "\n";
			++duration;
			if(duration >= motionComp.minDuration)
			{
				//Unless it's the first component in the input. We want, say, quarter circles to count even if the player was holding down for a long time before completing the motion
				//Or, charging back to count for all the frames after back has been held for the needed duration.
				if(firstInput)
				{
					debugString += "---Last Input: Early Out\n";
					return 0;
				}
				//To prevent different inputs being counted as the same with motions not marked strict, change the checks to return false if the input changes at all.
				if(motionComp.direction != 0)
				{
					motionComp.direction = inputBuffer->at(cind)->axisState;
					motionComp.strict = true;
				}
			}
			--cind;
		}
		else
		{//Only consider returning if the motion is invalid, which means we only check if the input has been valid after it's over
			debugString += "--Invalid Frame " + std::to_string(cind) + " dur:" + std::to_string(duration) + "|" + std::to_string((int)inputBuffer->at(cind)->axisState) + "," + std::to_string((int)inputBuffer->at(cind)->buttonStates) + "\n";
			if(duration >= motionComp.minDuration)
			{
				return glm::max(glm::abs(currentIndex - cind) - motionComp.minDuration + 1, 1);
			}
			else
			{
				duration = 0;
				--cind;
			}
		}
	} while(glm::abs(currentIndex - cind) - motionComp.minDuration <= maxBuffer);
	return -1;//-1 is used for invalid results.
}
bool InputDevice::InputMotionFrameCheck(InputMotionComponent& motionComp, int index)
{
	InputFrame* inpf = inputBuffer->at(index);//Current checked frame
	InputFrame* inpfp = inputBuffer->at(index - 1);//Current checked frame -1, to check for button events

	if(motionComp.direction != 0)
	{
		if((inpf->axisState&motionComp.direction) == 0)
			return false;
		if(motionComp.strict && (inpf->axisState != motionComp.direction))
			return false;
	}
	for(auto i = motionComp.buttons.begin(); i != motionComp.buttons.end(); ++i)
	{
		if(((int)(i->second) | (int)InputType::Pressed))
		{
			if((inpf->buttonStates&i->first) && ((inpfp->buttonStates&i->first) == 0))
				continue;
		}
		if(((int)(i->second) | (int)InputType::Released))
		{
			if((inpfp->buttonStates&i->first) && ((inpf->buttonStates&i->first) == 0))
				continue;
		}
		if(((int)(i->second) | (int)InputType::Held))
		{
			if((inpf->buttonStates&i->first))
				continue;
		}
		return false;
	}
	return true;
}

void InputDevice::UpdateJoyInputs()
{
	int count = 0;
	const float* axes = glfwGetJoystickAxes(this->_deviceID, &count);
	this->cachedJoyAxes = std::vector<float>(axes, axes + count);
	const unsigned char* buttons = glfwGetJoystickButtons(this->_deviceID, &count);
	this->cachedJoyButtons = std::vector<unsigned char>(buttons, buttons + count);
}
void InputDevice::PollInput()
{
	if(cachedInputFrames.size() == 0 || Time::lastUpdateTime + VE_FRAME_TIME*cachedInputFrames.size() < glfwGetTime())
	{
		cachedInputFrames.push_back(InputFrame(0, 0));
	}

	if(this->_deviceID>-1 && !glfwJoystickPresent(this->_deviceID))
		return;

	glm::ivec2 dir;
	cachedInputFrames.back().axisState = 0;
	for(auto i = directionMap.begin(); i != directionMap.end(); ++i)
	{
		if(EvaluateInput(i->second))
		{
			switch(i->first)
			{
				case InputDirection::Down:
					dir.y -= 1;
					break;
				case InputDirection::Left:
					dir.x -= 1;
					break;
				case InputDirection::Right:
					dir.x += 1;
					break;
				case InputDirection::Up:
					dir.y += 1;
					break;
			}
		}
	}

	if(dir.x > 0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Right);
	else if(dir.x<0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Left);
	if(dir.y>0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Up);
	else if(dir.y < 0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Down);

	for(auto i = buttonMap.begin(); i != buttonMap.end(); ++i)
	{
		if(EvaluateInput(i->second))
		{
			cachedInputFrames.back().buttonStates |= (unsigned char)(i->first);
		}
		//else
		//	cachedInputFrames.back().buttonStates &= ~((unsigned char)(i->first));
	}

}
void InputDevice::PushInputsToBuffer()
{
	for(unsigned int i = 0; i < cachedInputFrames.size(); ++i)
	{
		inputBuffer->Advance(cachedInputFrames[i]);
	}
	cachedInputFrames.clear();
}
//Default settings are saved per input device type. This means your fightstick can remember its settings even if you plug it out.
//Of course, support for temporary changes should be available in a finished product.
std::string InputDevice::Serialize()
{
	std::string str = "#BUTTONS\n";
	for(auto i = this->buttonMap.begin(); i != this->buttonMap.end(); ++i)
		str += std::to_string((int)i->first) + ":" + (i->second.isAxis ? "t," : "f,") + std::to_string(i->second.inputID) + "," + std::to_string((int)glm::sign(i->second.inputValue)) + "\n";
	str += "#AXES\n";
	for(auto i = this->directionMap.begin(); i != this->directionMap.end(); ++i)
		str += std::to_string((int)i->first) + ":" + (i->second.isAxis ? "t," : "f,") + std::to_string(i->second.inputID) + "," + std::to_string((int)glm::sign(i->second.inputValue)) + "\n";
	return str;
}
std::string InputDevice::deviceName()
{
	return _deviceName;
}
int InputDevice::deviceID()
{
	return _deviceID;
}