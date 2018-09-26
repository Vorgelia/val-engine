#include "InputDevice.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "Time.h"
#include "Screen.h"
#include "InputFrame.h"
#include "InputEvent.h"
#include "InputMotion.h"
#include "InputMotionComponent.h"
#include <GLM/glm.hpp>
#include "InputDeviceId.h"

//Default input leniency. We want the first input of every move to only count if it's the latest one by default.
#define INPUT_BUFFER_INIT 1
#define INPUT_BUFFER_MID 8
//1200 keeps track of the last 20 seconds of inputs, but it can easily be increased if that's not enough
#define VE_INPUT_BUFFER_SIZE 1200

const std::string& InputDevice::deviceName() const
{
	return _deviceName;
}

const InputBuffer* InputDevice::inputBuffer() const
{
	return _inputBuffer.get();
}

int InputDevice::deviceID() const
{
	return _deviceID;
}

InputDevice::InputDevice(int deviceID, GameInstance* serviceManager)
{
	_time = serviceManager->Time();
	_screen = serviceManager->Screen();
	_filesystem = serviceManager->Filesystem();

	_deviceID = deviceID;
	if(deviceID == (int)InputDeviceId::Network)
	{
		this->_deviceName = std::string("Network");
	}
	else if(deviceID == (int)InputDeviceId::Invalid)
	{
		this->_deviceName = std::string("Invalid");
	}
	else
	{
		if(deviceID == (int)InputDeviceId::Keyboard)
		{
			this->_deviceName = std::string("Keyboard");
		}
		else
		{
			this->_deviceName = std::string(glfwGetJoystickName(deviceID));
		}

		this->_deviceFilename = this->_deviceName;

		for(char& c : this->_deviceFilename)
		{
			if(std::string::npos != std::string("\\/:?\"<>|").find(c))
				c = '_';
		}

		_filesystem->LoadControlSettings("Settings/Input/" + this->_deviceFilename + ".vi", _directionMap, _buttonMap);
	}

	_inputBuffer = std::make_shared<InputBuffer>(VE_INPUT_BUFFER_SIZE);
}

InputDevice::~InputDevice()
= default;

//Helper function for evaluating whether a specific input event is occuring.
//Needed to abstract checking for buttons and axes to a single function with boolean output.
bool InputDevice::EvaluateInput(const InputEvent& ie)
{
	switch(_deviceID)
	{
	case (int)InputDeviceId::Network:
	case (int)InputDeviceId::Invalid:
		return false;
	case(int)InputDeviceId::Keyboard:
		return glfwGetKey(_screen->window, ie._inputID) == GLFW_PRESS;
	default:
		if(!ie._isAxis)
		{
			return _cachedJoyButtons[ie._inputID] > 0;
		}
		else
		{
			return (glm::abs(_cachedJoyAxes[ie._inputID]) > ie._deadzone) && (glm::sign(_cachedJoyAxes[ie._inputID]) == glm::sign(ie._inputValue));
		}
	}
}

//This function is fairly simple. It checks if a specific motion has been performed by validating every part of the motion based on the distance of its beginning.
//For a quarter circle forward L, it would check if L has been pressed, then the distance between L and a forward input, then the distance between the forward input to the down-forward input, etc
//The distance checking is abstracted in the InputMotionDistance function.
bool InputDevice::EvaluateMotion(const InputMotion& motion)
{
	if(motion._components.size() == 0)
		return false;

	int buf = motion._components.back().leniency > -1 ? motion._components.back().leniency : INPUT_BUFFER_INIT;
	int bufferIndex = _inputBuffer->position() - 1;

	for(int i = motion._components.size() - 1; i >= 0;)
	{
		int distance = InputMotionDistance(bufferIndex, motion._components[i], buf, i == 0);
		if(distance >= 0 && distance <= buf)
		{
			bufferIndex -= distance + motion._components[i].minDuration - 1;
			--i;
			if(i <= -1)
			{
				return true;
			}
			else
			{
				buf = motion._components[i].leniency > -1 ? motion._components[i].leniency : INPUT_BUFFER_MID;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}

//This function checks the distance from an input's beginning by checking back in the buffer until the specified input exceeds its necessary duration and stops being valid.
//For instance, holding forward in a quarter circle forward for 4 frames means the input is valid since it turns invalid after a 10f window.
//Max buffer is used as an early out to prevent from checking too far if we're going to discard based on distance being too high anyway
int InputDevice::InputMotionDistance(int currentIndex, InputMotionComponent motionComp, int maxBuffer, bool firstInput)
{
	int cind = currentIndex;
	int duration = 0;
	do
	{
		//Look back into the buffer and check if motionComp is valid for that frame. If it is, increase the duration of the input.
		//If the duration is over the needed duration, keep checking to see when the input started
		if(InputMotionFrameCheck(motionComp, cind))
		{
			++duration;
			if(duration >= motionComp.minDuration)
			{
				//Unless it's the first component in the input. We want, say, quarter circles to count even if the player was holding down for a long time before completing the motion
				//Or, charging back to count for all the frames after back has been held for the needed duration.
				if(firstInput)
				{
					return 0;
				}
				//To prevent different inputs being counted as the same with motions not marked strict, change the checks to return false if the input changes at all.
				if(motionComp.direction != 0)
				{
					motionComp.direction = _inputBuffer->at(cind)._axisState;
					motionComp.strict = true;
				}
			}
			--cind;
		}
		else
		{
			//Only consider returning if the motion is invalid, which means we only check if the input has been valid after it's over
			if(duration >= motionComp.minDuration)
			{
				return glm::max<int>(glm::abs(currentIndex - cind) - motionComp.minDuration + 1, 1);
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

bool InputDevice::InputMotionFrameCheck(const InputMotionComponent& motionComp, int index) const
{
	InputFrame& inputFrame = _inputBuffer->at(index);//Current checked frame
	InputFrame& inputFramePrevious = _inputBuffer->at(index - 1);//Current checked frame -1, to check for button events

	if(motionComp.direction != 0)
	{
		if((inputFrame._axisState&motionComp.direction) == 0)
			return false;
		if(motionComp.strict && (inputFrame._axisState != motionComp.direction))
			return false;
	}

	for(auto& i : motionComp.buttons)
	{
		if((int)i.second & (int)InputTypeMask::Pressed)
		{
			if(((inputFrame._buttonStates & i.first) != 0)
				&& ((inputFramePrevious._buttonStates & i.first) == 0))
				continue;
		}
		if((int)i.second & (int)InputTypeMask::Released)
		{
			if(((inputFramePrevious._buttonStates & i.first) != 0)
				&& ((inputFrame._buttonStates & i.first) == 0))
				continue;
		}
		if((int)i.second & (int)InputTypeMask::Held)
		{
			if((inputFrame._buttonStates & i.first))
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
	this->_cachedJoyAxes = std::vector<float>(axes, axes + count);
	const unsigned char* buttons = glfwGetJoystickButtons(this->_deviceID, &count);
	this->_cachedJoyButtons = std::vector<unsigned char>(buttons, buttons + count);
}

void InputDevice::PollInput()
{
	if(_cachedInputFrames.empty() || _time->lastUpdateTime + VE_FRAME_TIME * _cachedInputFrames.size() < glfwGetTime())
	{
		_cachedInputFrames.emplace_back(0, 0);
	}

	if(_deviceID >= (int)InputDeviceId::JoystickFirst && !glfwJoystickPresent(_deviceID))
		return;

	glm::ivec2 dir;
	_cachedInputFrames.back()._axisState = 0;
	for(auto& i : _directionMap)
	{
		if(EvaluateInput(i.second))
		{
			switch(i.first)
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
		_cachedInputFrames.back()._axisState |= (unsigned char)(InputDirection::Right);
	else if(dir.x < 0)
		_cachedInputFrames.back()._axisState |= (unsigned char)(InputDirection::Left);
	if(dir.y > 0)
		_cachedInputFrames.back()._axisState |= (unsigned char)(InputDirection::Up);
	else if(dir.y < 0)
		_cachedInputFrames.back()._axisState |= (unsigned char)(InputDirection::Down);

	for(auto& i : _buttonMap)
	{
		if(EvaluateInput(i.second))
		{
			_cachedInputFrames.back()._buttonStates |= (unsigned char)(i.first);
		}
	}

}

void InputDevice::PushInputsToBuffer()
{
	for(auto& frame : _cachedInputFrames)
	{
		_inputBuffer->Advance(frame);
	}
	_cachedInputFrames.clear();
}

//Default settings are saved per input device type. This means your fightstick can remember its settings even if you plug it out.
//Of course, support for temporary changes should be available in a finished product.
std::string InputDevice::Serialize()
{
	std::stringstream str;
	str << "#BUTTONS\n";
	for(auto& i : _buttonMap)
		str << (int)i.first << ":" << (i.second._isAxis ? "t," : "f,") << i.second._inputID << "," << (int)glm::sign(i.second._inputValue) << "\n";

	str << "#AXES\n";
	for(auto& i : _directionMap)
		str << (int)i.first << ":" << (i.second._isAxis ? "t," : "f,") << i.second._inputID << "," << (int)glm::sign(i.second._inputValue) << "\n";
	return str.str();
}
