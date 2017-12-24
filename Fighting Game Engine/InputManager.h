#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include "BaseService.h"
#include "GLIncludes.hpp"
#include "Delegate.h"

class InputDevice;

class InputManager : public BaseService
{
private:
	std::unordered_map<int, std::shared_ptr<InputDevice>> _inputDevices;
	std::vector<std::unique_ptr<InputDevice>> _temporaryNetworkDevices;
	std::thread _inputCollectionThread;
	bool _stopInputs = false;

public:
	typedef Delegate<InputDevice*> DeviceEventHandler;
	DeviceEventHandler DeviceAdded;
	DeviceEventHandler DeviceRemoved;

	void FrameUpdate();
	void Update();

	//TODO: Create temporary network device
	const std::unordered_map<int, std::shared_ptr<InputDevice>>& inputDevices();

	InputDevice* GetInputDevice(int id);
	InputDevice* GetTemporaryNetworkDevice();
	void ReleaseTemporaryNetworkDevice(InputDevice* device);

	InputManager(ServiceManager* serviceManager);
	~InputManager();
};