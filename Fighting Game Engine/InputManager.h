#pragma once
#include <unordered_map>
#include <memory>
#include <thread>
#include "BaseService.h"
#include "InputDevice.h"
#include "Delegate.h"

class InputDevice;
class ScreenManager;
struct InputEvent;

class InputManager : public BaseService
{
	VE_OBJECT_DECLARATION(InputManager);

protected:
	ScreenManager* _screenManager;

protected:
	std::unordered_map<int, ve::unique_object_ptr<InputDevice>> _inputDevices;
	std::vector<ve::unique_object_ptr<InputDevice>> _temporaryNetworkDevices;
	std::thread _inputCollectionThread;
	bool _stopInputs = false;

	InputDevice* AddInputDevice(int deviceID);

public:
	typedef Delegate<InputDevice*> DeviceEventHandler;
	DeviceEventHandler DeviceAdded;
	DeviceEventHandler DeviceRemoved;

	virtual void OnInit() override;
	void OnServiceInit() override;
	virtual void OnDestroyed() override;

	void UpdateInputs();

	const std::unordered_map<int, ve::unique_object_ptr<InputDevice>>& inputDevices() const { return _inputDevices; }

	bool IsKeyboardKeyPressed(int keyID) const;
	bool EvaluateInput(InputDevice* inputDevice, const InputEvent& inputEvent) const;

	InputDevice* GetInputDevice(int id);
	InputDevice* GetTemporaryNetworkDevice();
	void ReleaseTemporaryNetworkDevice(InputDevice* device);

	InputManager() = default;
	~InputManager() = default;
};