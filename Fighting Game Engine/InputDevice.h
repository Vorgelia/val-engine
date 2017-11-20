#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "InputFrame.h"
#include "InputEvent.h"
#include "CircularBuffer.h"
#include "InputIdentifiers.h"
#include "GLIncludes.hpp"

class InputMotion;
class InputMotionComponent;
typedef CircularBuffer<InputFrame> InputBuffer;

enum class InputDeviceId
{
	Network = -3,
	Invalid = -2,
	Keyboard = -1,
	JoystickFirst = GLFW_JOYSTICK_1,
	JoystickLast = GLFW_JOYSTICK_LAST
};

class InputDevice
{
	friend class GamePlayer;

	int _deviceID;
	std::string _deviceName;
	std::string _deviceFilename;
	std::vector<InputFrame> _cachedInputFrames;

	std::vector<unsigned char> _cachedJoyButtons;
	std::vector<float> _cachedJoyAxes;

	std::unordered_map<InputButton, InputEvent> _buttonMap;
	std::unordered_map<InputDirection, InputEvent> _directionMap;

	std::shared_ptr<InputBuffer> _inputBuffer;

	int InputMotionDistance(int currentIndex, InputMotionComponent motionComp, int maxBuffer = 1199, bool firstInput = false);
public:
	
	const int& deviceID() const;
	const std::string& deviceName() const;
	const InputBuffer* inputBuffer() const;

	void PollInput();
	void UpdateJoyInputs();
	void PushInputsToBuffer();
	
	std::string Serialize();

	bool EvaluateInput(const InputEvent& ie);
	bool EvaluateMotion(const InputMotion& motion);
	bool InputMotionFrameCheck(const InputMotionComponent& motionComp, int index);

	InputDevice(int deviceID);
	~InputDevice();
};