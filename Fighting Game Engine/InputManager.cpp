#include "InputManager.h"
#include <unordered_set>
#include "InputDevice.h"
#include "InputFrame.h"
#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
#include "Screen.h"

//Update list of valid input devices and have them poll for inputs
void InputManager::FrameUpdate()
{
	glfwPollEvents();
	for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
	{
		if(glfwJoystickPresent(i) == GLFW_TRUE && (_inputDevices.count(i) == 0))
		{
			InputDevice* addedDevice = _inputDevices.insert(std::pair<int, std::shared_ptr<InputDevice>>(i, std::make_shared<InputDevice>(i))).first->second.get();
			DeviceAdded(addedDevice);
		}
		else if(glfwJoystickPresent(i) == GLFW_FALSE)
		{
			auto& iter = _inputDevices.find(i);
			if(iter != _inputDevices.end())
			{
				DeviceRemoved(iter->second.get());
				_inputDevices.erase(iter);
			}
		}
	}

	for(auto& i : _inputDevices)
	{
		if(i.second != nullptr)
		{
			i.second->UpdateJoyInputs();
			i.second->PollInput();
			i.second->PushInputsToBuffer();
		}
	}
}

void InputManager::Update()
{
}

const std::unordered_map<int, std::shared_ptr<InputDevice>>& InputManager::inputDevices()
{
	return _inputDevices;
}

InputDevice* InputManager::GetInputDevice(int id)
{
	switch(id)
	{
	case (int)InputDeviceId::Invalid:
	case (int)InputDeviceId::Network:
		return nullptr;
	default:
		auto& iter = _inputDevices.find(id);
		if(iter == _inputDevices.end())
		{
			return nullptr;
		}

		return iter->second.get();
	}
}

InputDevice* InputManager::GetTemporaryNetworkDevice()
{
	_temporaryNetworkDevices.emplace_back(std::make_unique<InputDevice>((int)InputDeviceId::Network));
	return _temporaryNetworkDevices.back().get();
}

void InputManager::ReleaseTemporaryNetworkDevice(InputDevice* device)
{
	for(auto iter = _temporaryNetworkDevices.begin(); iter != _temporaryNetworkDevices.end(); ++iter)
	{
		if(iter->get() == device)
		{
			_temporaryNetworkDevices.erase(iter);
		}
	}
}

InputManager::InputManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	_inputDevices[(int)InputDeviceId::Keyboard] = std::make_shared<InputDevice>((int)InputDeviceId::Keyboard);
}

InputManager::~InputManager()
{
	_inputDevices.clear();
	_temporaryNetworkDevices.clear();
}
