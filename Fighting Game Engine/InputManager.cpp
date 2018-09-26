#include "InputManager.h"
#include <unordered_set>
#include "InputDevice.h"
#include "GLIncludes.hpp"
#include "FilesystemManager.h"
#include "InputDeviceId.h"

//Update list of valid input devices and have them poll for inputs
void InputManager::FrameUpdate()
{
	glfwPollEvents();
	for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
	{
		if(glfwJoystickPresent(i) == GLFW_TRUE && (_inputDevices.count(i) == 0))
		{
			InputDevice* addedDevice = _inputDevices.insert(std::pair<int, std::shared_ptr<InputDevice>>(i, std::make_shared<InputDevice>(i, _serviceManager))).first->second.get();
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

void InputManager::Init()
{
	_inputDevices[(int)InputDeviceId::Keyboard] = std::make_shared<InputDevice>((int)InputDeviceId::Keyboard, _serviceManager);
}

void InputManager::Update()
{
}

void InputManager::Cleanup()
{
	_inputDevices.clear();
	_temporaryNetworkDevices.clear();
}

const std::unordered_map<int, std::shared_ptr<InputDevice>>& InputManager::inputDevices() const
{
	return _inputDevices;
}

InputDevice* InputManager::GetInputDevice(int id)
{
	switch(id)
	{
	case (int)InputDeviceId::Invalid:
		return nullptr;
	case (int)InputDeviceId::Network:
		return GetTemporaryNetworkDevice();
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
	_temporaryNetworkDevices.emplace_back(std::make_unique<InputDevice>((int)InputDeviceId::Network, _serviceManager));
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

InputManager::InputManager(GameInstance* serviceManager) : BaseService(serviceManager)
{
}

InputManager::~InputManager()
= default;
