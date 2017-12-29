#include "GamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"
#include "ServiceManager.h"
#include "Delegate.h"

void GamePlayer::Update()
{

}

void GamePlayer::HandleDeviceRemoved(InputDevice* device)
{
	if(device == _inputDevice)
	{
		_inputDevice = nullptr;
	}
}

InputDevice * GamePlayer::inputDevice()
{
	return _inputDevice;
}

void GamePlayer::SetDevice(int deviceId)
{
	_inputDevice = _input->GetInputDevice(deviceId);
}

GamePlayer::GamePlayer(int id, int deviceId, ServiceManager* serviceManager)
{
	_input = serviceManager->Input();

	_id = id;
	SetDevice(deviceId);

	_input->DeviceRemoved += InputManager::DeviceEventHandler::func_t([this](InputDevice* device) { HandleDeviceRemoved(device); });
}

GamePlayer::~GamePlayer()
{
}
