#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "InputFrame.h"
#include "InputEvent.h"
#include "CircularBuffer.h"
#include "InputIdentifiers.h"
#include "Delegate.h"
#include "BaseObject.h"

class FilesystemManager;
class GameInstance;
class InputMotion;
class InputMotionComponent;
typedef CircularBuffer<InputFrame> InputBuffer;

class InputDevice : public BaseObject
{
	friend class GamePlayer;
	friend class InputManager;

	VE_OBJECT_DECLARATION(InputDevice)

private:
	InputManager* _inputManager;
	FilesystemManager* _filesystem;

protected:
	int _deviceID;
	std::string _deviceName;
	std::string _deviceFilename;

	std::vector<unsigned char> _cachedJoyButtons;
	std::vector<float> _cachedJoyAxes;

	std::unordered_map<InputButton, InputEvent> _buttonMap;
	std::unordered_map<InputDirection, InputEvent> _directionMap;

	std::vector<InputFrame> _cachedInputFrames;
	std::shared_ptr<InputBuffer> _inputBuffer;

	int InputMotionDistance(int currentIndex, InputMotionComponent motionComp, int maxBuffer, bool firstInput = false);

public:
	int deviceID() const;
	const std::string& deviceName() const;
	const InputBuffer* inputBuffer() const;

	virtual void OnInit() override;
	virtual void OnDestroyed() override;

	void PollInput();
	void UpdateJoyInputs();
	void PushInputsToBuffer();
	
	std::string SerializeSettings();

	bool EvaluateInput(const InputEvent& ie);
	bool EvaluateMotion(const InputMotion& motion);
	bool InputMotionFrameCheck(const InputMotionComponent& motionComp, int index) const;

	InputDevice();
	~InputDevice() = default;
};