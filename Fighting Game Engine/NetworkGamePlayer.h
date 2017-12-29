#pragma once
#include "GamePlayer.h"
class NetworkGamePlayer :
	public GamePlayer
{
	//TODO: something about a network connection

	virtual void Update() override;

public:

	NetworkGamePlayer(int id, ServiceManager* serviceManager);
	virtual ~NetworkGamePlayer();
};