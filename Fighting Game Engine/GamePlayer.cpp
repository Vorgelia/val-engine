#include "GamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"
#include "GameInstance.h"
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

InputDevice * GamePlayer::inputDevice() const
{
	return _inputDevice;
}

void GamePlayer::SetDevice(int deviceId)
{
	_inputDevice = _input->GetInputDevice(deviceId);
}

GamePlayer::GamePlayer(int id, int deviceId, GameInstance* serviceManager)
{
	_input = serviceManager->Input();

	_id = id;
	SetDevice(deviceId);

	//_input->DeviceRemoved += VE_DELEGATE_FUNC(InputManager::DeviceEventHandler, HandleDeviceRemoved);
}

GamePlayer::~GamePlayer()
= default;
