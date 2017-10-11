#include "InputManager.h"
#include "InputDevice.h"
#include "InputFrame.h"
#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
#include "Screen.h"

namespace InputManager
{
	std::unordered_map<int, std::shared_ptr<InputDevice>> _inputDevices;
	std::thread _inputCollectionThread;
	int _p1Device;
	int _p2Device;
	bool _stopInputs = false;
}

//Update list of valid input devices and have them poll for inputs
void InputManager::Update()
{
	glfwPollEvents();
	//stopInputs = true;
	//inputCollectionThread.join();
	for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
	{
		if(glfwJoystickPresent(i) == GLFW_TRUE && (_inputDevices.count(i) == 0))
		{
			_inputDevices.insert(std::pair<int, std::shared_ptr<InputDevice>>(i, std::make_shared<InputDevice>(i)));
		}
		else if(glfwJoystickPresent(i) == GLFW_FALSE && (_inputDevices.count(i) > 0))
		{
			_inputDevices.erase(i);
			if(_p1Device == i)
				_p1Device = -2;
			if(_p2Device == i)
				_p2Device = -2;
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
	//inputCollectionThread = std::thread(BufferInputs);
}

void InputManager::Init()
{
	_inputDevices[-1] = std::make_shared<InputDevice>(-1);
	_p1Device = -1;
	_p2Device = 0;
}

void InputManager::Cleanup()
{
	_inputDevices.clear();
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "---Key:" << glfwGetTime()<<"/"<<Time::lastUpdateTime << " - " << (glfwGetKey(Screen::window, key) == GLFW_PRESS) << std::endl;
}