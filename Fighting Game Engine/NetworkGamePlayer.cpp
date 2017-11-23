#include "NetworkGamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"

void NetworkGamePlayer::Update()
{
	//TODO: Update input device on network input
}

NetworkGamePlayer::NetworkGamePlayer(GamePlayerType type) : GamePlayer(type, (int)InputDeviceId::Network)
{
	_inputDevice = InputManager::GetTemporaryNetworkDevice();
}

NetworkGamePlayer::~NetworkGamePlayer()
{
}
