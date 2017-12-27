#include "NetworkGamePlayer.h"
#include "InputManager.h"
#include "InputDevice.h"

void NetworkGamePlayer::Update()
{
	//TODO: Update input device on network input
}

NetworkGamePlayer::NetworkGamePlayer(GamePlayerType type, ServiceManager* serviceManager) : GamePlayer(type, serviceManager, (int)InputDeviceId::Network)
{
	_inputDevice = _input->GetTemporaryNetworkDevice();
}

NetworkGamePlayer::~NetworkGamePlayer()
{
}
