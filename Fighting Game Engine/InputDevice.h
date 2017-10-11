#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "InputFrame.h"
#include "InputEvent.h"
#include "CircularBuffer.h"
#include "Time.h"

class InputMotionComponent;
typedef std::vector<InputMotionComponent> InputMotion;
typedef CircularBuffer<InputFrame> InputBuffer;

enum class InputButton
{
	Light = 1,
	Medium = 2,
	Heavy = 4,
	Attacks = 7,//Convenience entry that represents any of the buttons
	Shield = 8,
	Any = 15,//Convenience entry that represents any button
	Select = 16,
	Start = 32
};

enum class InputDirection
{
	Up = 1,
	Down = 2,
	Right = 4,
	Left = 8
};

class InputDevice
{
	int _deviceID;
	std::string _deviceName;
	std::string _deviceFilename;
	std::vector<InputFrame> _cachedInputFrames;

	std::vector<unsigned char> _cachedJoyButtons;
	std::vector<float> _cachedJoyAxes;

	std::unordered_map<InputButton, InputEvent> _buttonMap;
	std::unordered_map<InputDirection, InputEvent> _directionMap;

	std::shared_ptr<InputBuffer> _inputBuffer;
public:
	
	int deviceID();
	std::string deviceName();
	const std::shared_ptr<InputBuffer> inputBuffer();

	void PollInput();
	void UpdateJoyInputs();
	void PushInputsToBuffer();
	
	std::string Serialize();


	bool EvaluateInput(InputEvent& ie);
	bool EvaluateMotion(InputMotion& motion, bool inverse);
	int InputMotionDistance(int currentIndex, InputMotionComponent& motionComp, int maxBuffer = 1199, bool firstInput = false);
	bool InputMotionFrameCheck(InputMotionComponent& motionComp, int index);
	InputDevice(int deviceID);
	~InputDevice();
};