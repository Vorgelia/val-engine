#include "PlayerManager.h"
#include "GamePlayer.h"
#include "NetworkGamePlayer.h"

#define VE_MAX_PLAYERS 16

GamePlayer* PlayerManager::AddPlayer(int id, int inputDeviceId)
{
	_players[id] = std::move(std::make_unique<GamePlayer>(id, inputDeviceId, _serviceManager));
	PlayerAdded(_players[id].get());
	return _players[id].get();
}

GamePlayer* PlayerManager::AddNetworkPlayer(int id)
{
	_players[id] = std::move(std::make_unique<NetworkGamePlayer>(id, _serviceManager));
	PlayerAdded(_players[id].get());
	return _players[id].get();
}

GamePlayer* PlayerManager::GetPlayer(int id)
{
	return _players[id].get();
}

void PlayerManager::RemovePlayer(int id)
{
	PlayerRemoved(_players[id].get());
	_players[id] = nullptr;
}

void PlayerManager::ClearPlayers()
{
	for(size_t i = 0; i < _players.size(); ++i)
	{
		RemovePlayer(i);
	}
}

void PlayerManager::Init()
{
}

void PlayerManager::Update()
{
	for(size_t i = 0; i < _players.size(); ++i)
	{
		if(_players[i] != nullptr)
		{
			_players[i]->Update();
		}
	}
}

void PlayerManager::Cleanup()
{
	ClearPlayers();
}

PlayerManager::PlayerManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	_allowServiceUpdate = true;
	_players.resize(VE_MAX_PLAYERS);
}

PlayerManager::~PlayerManager()
{
}
