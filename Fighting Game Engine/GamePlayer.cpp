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

GamePlayer::GamePlayer(GamePlayerType type, ServiceManager* serviceManager, int deviceId)
{
	_input = serviceManager->Input();
	_type = type;
	//TODO
	_inputDevice = _input->GetInputDevice(deviceId);
	_input->DeviceRemoved += InputManager::DeviceEventHandler::func_t([this](InputDevice* device) { HandleDeviceRemoved(device); });
}

GamePlayer::~GamePlayer()
{
}
