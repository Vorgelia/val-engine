#include "InputManager.h"
#include <unordered_set>
#include "InputDevice.h"
#include "GLIncludes.hpp"
#include "FilesystemManager.h"
#include "InputDeviceId.h"
#include "InputEvent.h"
#include "TrackedUpdateFunction.h"
#include "ScreenManager.h"
#include "GameInstance.h"

VE_OBJECT_DEFINITION(InputManager);

InputDevice* InputManager::AddInputDevice(int deviceID)
{
	InputDevice* inputDevice;

	switch(deviceID)
	{
		case int(InputDeviceId::Network) :
			inputDevice = _temporaryNetworkDevices.emplace_back(ObjectFactory::CreateObjectDeferred<InputDevice>()).get();
			break;
		default:
			inputDevice = _inputDevices.emplace(deviceID, ObjectFactory::CreateObjectDeferred<InputDevice>()).first->second.get();;
			break;
	}
	
	inputDevice->_deviceID = deviceID;

	ObjectFactory::InitializeObject(inputDevice, this);
	return inputDevice;
}

void InputManager::OnInit()
{
	_screenManager = _owningInstance->ScreenManager();

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::TimingUpdate, UpdateType::AnyFixedGameUpdate, UpdateInputs);
}

void InputManager::OnServiceInit()
{
	AddInputDevice(int(InputDeviceId::Keyboard));
}

void InputManager::OnDestroyed()
{
	_inputDevices.clear();
	_temporaryNetworkDevices.clear();
}

void InputManager::UpdateInputs()
{
	glfwPollEvents();
	for(int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i)
	{
		if(glfwJoystickPresent(i) == GLFW_TRUE && (_inputDevices.count(i) == 0))
		{
			InputDevice* addedDevice = AddInputDevice(i);
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

bool InputManager::IsKeyboardKeyPressed(int keyID) const
{
	return glfwGetKey(_screenManager->window(), keyID) == GLFW_PRESS;
}

bool InputManager::EvaluateInput(InputDevice* inputDevice, const InputEvent& inputEvent) const
{
	if(inputDevice == nullptr)
	{
		return false;
	}

	switch(inputDevice->_deviceID)
	{
	case int(InputDeviceId::Network) :
	case int(InputDeviceId::Invalid) :
		return false;

	case int(InputDeviceId::Keyboard) :
		return glfwGetKey(_screenManager->window(), inputEvent.inputID) == GLFW_PRESS;

	default:
		if(!inputEvent.isAxis)
		{
			return inputDevice->_cachedJoyButtons[inputEvent.inputID] > 0;
		}

		return (glm::abs(inputDevice->_cachedJoyAxes[inputEvent.inputID]) > inputEvent.deadzone) && (glm::sign(inputDevice->_cachedJoyAxes[inputEvent.inputID]) == glm::sign(inputEvent.inputValue));
	}
}

InputDevice* InputManager::GetInputDevice(int id)
{
	switch(id)
	{
	case int(InputDeviceId::Invalid):
		return nullptr;
	case int(InputDeviceId::Network) :
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
	return AddInputDevice(int(InputDeviceId::Network));
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