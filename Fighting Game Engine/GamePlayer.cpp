#include "GamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"

void GamePlayer::Update()
{

}

GamePlayer::GamePlayer(GamePlayerType type, int deviceId)
{
	_type = type;
	_inputDevice = InputManager::GetInputDevice(deviceId);
	if(_inputDevice != nullptr)
	{
		_inputDevice->DeviceRemoved += InputDevice::EventHandler::func_t([this]() { _inputDevice = nullptr; });
	}
}

GamePlayer::~GamePlayer()
{
}
