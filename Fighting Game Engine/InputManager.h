#pragma once
#include <unordered_map>
#include <memory>
#include "GLIncludes.hpp"

class InputDevice;

namespace InputManager
{
	extern std::unordered_map<int, std::shared_ptr<InputDevice>> _inputDevices;

	void Init();
	void Update();
	void Cleanup();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
