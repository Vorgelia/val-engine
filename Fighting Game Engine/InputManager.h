#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include "GLIncludes.hpp"
#include "Delegate.h"

class InputDevice;

namespace InputManager
{
	typedef Delegate<InputDevice*> DeviceEventHandler;
	extern DeviceEventHandler DeviceAdded;
	extern DeviceEventHandler DeviceRemoved;

	void Init();
	void Update();
	void Cleanup();

	//TODO: Create temporary network device
	const std::unordered_map<int, std::shared_ptr<InputDevice>>& inputDevices();

	InputDevice* GetInputDevice(int id);
	InputDevice* GetTemporaryNetworkDevice();
	void ReleaseTemporaryNetworkDevice(InputDevice* device);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}