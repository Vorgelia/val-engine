#include "InputDevice.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "ScreenManager.h"
#include "InputFrame.h"
#include "InputManager.h"
#include "InputEvent.h"
#include "InputMotion.h"
#include "InputMotionComponent.h"
#include <GLM/glm.hpp>
#include "InputDeviceId.h"

VE_OBJECT_DEFINITION(InputDevice);

const std::string& InputDevice::deviceName() const
{
	return _deviceName;
}

const InputBuffer* InputDevice::inputBuffer() const
{
	return _inputBuffer.get();
}

void InputDevice::OnInit()
{
	_inputManager = _owningInstance->Input();
	_filesystem = _owningInstance->Filesystem();

	_inputBuffer = std::make_shared<InputBuffer>(_owningInstance->configData().inputConfigData.inputBufferSize);

	switch(_deviceID)
	{
	case (int)InputDeviceId::Invalid:
		_deviceName = "Invalid";
		return;
	case (int)InputDeviceId::Network:
		_deviceName = "Network";
		return;
	case (int)InputDeviceId::Keyboard:
		_deviceName = std::string("Keyboard");
		break;
	default:
		_deviceName = std::string(glfwGetJoystickName(_deviceID));
		break;
	}
	
	_deviceFilename = _deviceName;

	for(char& c : _deviceFilename)
	{
		if(std::string("\\/:?\"<>|").find(c) != std::string::npos)
			c = '_';
	}

	_filesystem->LoadControlSettings("Settings/Input/" + _deviceFilename + ".vi", _directionMap, _buttonMap);
}
void InputDevice::OnDestroyed()
{
	
}

int InputDevice::deviceID() const
{
	return _deviceID;
}

//Helper function for evaluating whether a specific input event is occuring.
//Needed to abstract checking for buttons and axes to a single function with boolean output.
bool InputDevice::EvaluateInput(const InputEvent& ie)
{
	return _inputManager->EvaluateInput(this, ie);
}

//This function is fairly simple. It checks if a specific motion has been performed by validating every part of the motion based on the distance of its beginning.
//For a quarter circle forward L, it would check if L has been pressed, then the distance between L and a forward input, then the distance between the forward input to the down-forward input, etc
//The distance checking is abstracted in the InputMotionDistance function.
bool InputDevice::EvaluateMotion(const InputMotion& motion)
{
	if(motion._components.size() == 0)
		return false;

	int buf = motion._components.back().leniency > -1 ? motion._components.back().leniency : _owningInstance->configData().inputConfigData.inputBufferLeniencyFirst;
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
				buf = motion._components[i].leniency > -1 ? motion._components[i].leniency : _owningInstance->configData().inputConfigData.inputBufferLeniencyAny;
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
				if(motionComp.direction != InputDirection::None)
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

	if(motionComp.strict)
	{
		if(inputFrame._axisState != motionComp.direction)
		{
			return false;
		}
	}
	else if((motionComp.direction != InputDirection::None) && (inputFrame._axisState & motionComp.direction) == InputDirection::None)
	{
		return false;
	}

	for(auto& i : motionComp.buttons)
	{
		if((i.second & InputTypeMask::Pressed) != InputTypeMask::None)
		{
			if(((inputFrame._buttonStates & i.first) != InputButton::None)
				&& ((inputFramePrevious._buttonStates & i.first) == InputButton::None))
				continue;
		}
		if((i.second & InputTypeMask::Released) != InputTypeMask::None)
		{
			if(((inputFramePrevious._buttonStates & i.first) != InputButton::None)
				&& ((inputFrame._buttonStates & i.first) == InputButton::None))
				continue;
		}
		if((i.second & InputTypeMask::Held) != InputTypeMask::None)
		{
			if((inputFrame._buttonStates & i.first) != InputButton::None)
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
	_cachedInputFrames.emplace_back(0, 0);
	if(_deviceID >= int(InputDeviceId::JoystickFirst) && !glfwJoystickPresent(_deviceID))
		return;

	glm::ivec2 dir;
	_cachedInputFrames.back()._axisState = InputDirection::None;
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
			default:
				break;
			}
		}
	}

	if(dir.x > 0)
		_cachedInputFrames.back()._axisState |= InputDirection::Right;
	else if(dir.x < 0)
		_cachedInputFrames.back()._axisState |= InputDirection::Left;
	if(dir.y > 0)
		_cachedInputFrames.back()._axisState |= InputDirection::Up;
	else if(dir.y < 0)
		_cachedInputFrames.back()._axisState |= InputDirection::Down;

	for(auto& i : _buttonMap)
	{
		if(EvaluateInput(i.second))
		{
			_cachedInputFrames.back()._buttonStates |= i.first;
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

std::string InputDevice::SerializeSettings()
{
	std::stringstream str;
	str << "#BUTTONS\n";
	for(auto& i : _buttonMap)
		str << (int)i.first << ":" << (i.second.isAxis ? "t," : "f,") << i.second.inputID << "," << (int)glm::sign(i.second.inputValue) << "\n";

	str << "#AXES\n";
	for(auto& i : _directionMap)
		str << (int)i.first << ":" << (i.second.isAxis ? "t," : "f,") << i.second.inputID << "," << (int)glm::sign(i.second.inputValue) << "\n";
	return str.str();
}
