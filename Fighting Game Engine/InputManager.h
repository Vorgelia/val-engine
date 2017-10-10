#pragma once
#include "CommonUtilIncludes.hpp"

class InputDevice;

namespace InputManager
{
	extern std::map<int, std::shared_ptr<InputDevice>> _inputDevices;

	void Init();
	void Update();
	void Cleanup();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
