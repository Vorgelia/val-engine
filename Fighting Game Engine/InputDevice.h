#pragma once
#include "CommonUtilIncludes.hpp"
#include "InputFrame.h"
#include "InputEvent.h"
#include "CircularBuffer.h"
#include "Time.h"

class InputMotionComponent;
typedef std::vector<InputMotionComponent> InputMotion;
typedef CircularBuffer<InputFrame> InputBuffer;
enum class InputButton{
	Light = 1,
	Medium = 2,
	Heavy = 4,
	Buttons = 7,//Convenience entry that represents any of the buttons
	Shield = 8,
	Any = 15,//Convenience entry that represents any button
	Select = 16,
	Start = 32

};
enum class InputDirection{
	Up = 1,
	Down = 2,
	Right = 4,
	Left = 8
};


class InputDevice
{
	int _deviceID;
	std::string _deviceName;
	std::vector<InputFrame> cachedInputFrames;

	std::vector<unsigned char> cachedJoyButtons;
	std::vector<float> cachedJoyAxes;
	
public:
	std::unordered_map<InputButton, InputEvent> buttonMap;
	std::unordered_map<InputDirection, InputEvent> directionMap;

	InputBuffer* inputBuffer;

	void PollInput();
	void UpdateJoyInputs();
	void PushInputsToBuffer();
	int deviceID();
	std::string deviceName();
	std::string Serialize();

	bool EvaluateInput(InputEvent ie);
	bool EvaluateMotion(InputMotion motion, bool inverse);
	int InputMotionDistance(int currentIndex, InputMotionComponent motionComp, int maxBuffer = 1199, bool firstInput = false);
	bool InputMotionFrameCheck(InputMotionComponent* motionComp,int index);
	InputDevice(int deviceID);
	~InputDevice();
};