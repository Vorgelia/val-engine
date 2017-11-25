#include "GamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"

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

GamePlayer::GamePlayer(GamePlayerType type, int deviceId)
{
	_type = type;
	_inputDevice = InputManager::GetInputDevice(deviceId);
	InputManager::DeviceRemoved += InputManager::DeviceEventHandler::func_t([this](InputDevice* device) { HandleDeviceRemoved(device); });
}

GamePlayer::~GamePlayer()
{
}
