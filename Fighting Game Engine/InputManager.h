#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "GLIncludes.hpp"

class InputDevice;

namespace InputManager
{
	void Init();
	void Update();
	void Cleanup();

	//TODO: Create temporary network device
	const std::unordered_map<int, std::shared_ptr<InputDevice>>& inputDevices();
	InputDevice* GetInputDevice(int id);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}