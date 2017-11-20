#include "GamePlayer.h"
#include "InputManager.h"


void GamePlayer::SetDeviceId(int deviceId)
{
	_currentDevice = InputManager::GetInputDevice(deviceId);
}

GamePlayer::GamePlayer()
{
}

GamePlayer::~GamePlayer()
{
}
