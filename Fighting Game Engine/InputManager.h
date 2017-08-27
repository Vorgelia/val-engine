#pragma once
#include "CommonUtilIncludes.hpp"
//Including all the input-related .h files here since they won't be changed at all after they're done.
//Makes including all of them in other files less of a mess since they need to be along this one anyway

class InputDevice;

namespace InputManager
{
	extern std::map<int, InputDevice*> inputDevices;

	void Init();
	void Update();
	void BufferInputs();
	void Cleanup();
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
