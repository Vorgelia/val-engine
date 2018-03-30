#include "NetworkGamePlayer.h"
#include "InputManager.h"
#include "InputDeviceId.h"

void NetworkGamePlayer::Update()
{
	//TODO: Update input device on network input
}

NetworkGamePlayer::NetworkGamePlayer(int id, ServiceManager* serviceManager) : GamePlayer(id, (int)InputDeviceId::Network, serviceManager)
{
}

NetworkGamePlayer::~NetworkGamePlayer()
{
	_input->ReleaseTemporaryNetworkDevice(_inputDevice);
}
