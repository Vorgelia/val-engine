#include "InputManager.h"
#include <unordered_set>
#include "InputDevice.h"
#include "InputFrame.h"
#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
#include "Screen.h"

namespace InputManager
{
	std::unordered_map<int, std::shared_ptr<InputDevice>> _inputDevices;
	std::thread _inputCollectionThread;
	bool _stopInputs = false;
}

//Update list of valid input devices and have them poll for inputs
void InputManager::Update()
{
	glfwPollEvents();
	for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
	{
		if(glfwJoystickPresent(i) == GLFW_TRUE && (_inputDevices.count(i) == 0))
		{
			_inputDevices.insert(std::pair<int, std::shared_ptr<InputDevice>>(i, std::make_shared<InputDevice>(i)));
		}
		else if(glfwJoystickPresent(i) == GLFW_FALSE && (_inputDevices.count(i) > 0))
		{
			_inputDevices.erase(i);
		}
	}

	for(auto i : _inputDevices)
	{
		if(i.second != nullptr)
		{
			i.second->UpdateJoyInputs();
			i.second->PollInput();
			i.second->PushInputsToBuffer();
		}
	}
}

void InputManager::Init()
{
	_inputDevices[(int)InputDeviceId::Keyboard] = std::make_shared<InputDevice>((int)InputDeviceId::Keyboard);
}

void InputManager::Cleanup()
{
	_inputDevices.clear();
}

const std::unordered_map<int, std::shared_ptr<InputDevice>>& InputManager::inputDevices()
{
	return _inputDevices;
}

InputDevice* InputManager::GetInputDevice(int id)
{
	auto& iter = _inputDevices.find(id);
	if(iter == _inputDevices.end())
	{
		return nullptr;
	}

	return iter->second.get();
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "---Key:" << glfwGetTime()<<"/"<<Time::lastUpdateTime << " - " << (glfwGetKey(Screen::window, key) == GLFW_PRESS) << std::endl;
}