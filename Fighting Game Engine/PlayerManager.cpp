#include "PlayerManager.h"
#include "GamePlayer.h"
#include "NetworkGamePlayer.h"
#include "GameInstance.h"

#define VE_MAX_PLAYERS 16

GamePlayer* PlayerManager::AddPlayer(int id, int inputDeviceId)
{
	_players[id] = std::make_unique<GamePlayer>(id, inputDeviceId, _owningInstance);
	PlayerAdded(_players[id].get());
	return _players[id].get();
}

GamePlayer* PlayerManager::AddNetworkPlayer(int id)
{
	_players[id] = std::make_unique<NetworkGamePlayer>(id, _owningInstance);
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

void PlayerManager::OnInit()
{
	_owningInstance->updateDispatcher().BindFunction(
		this,
		UpdateFunctionTiming(int(UpdateGroup::FrameUpdate) - 20, UpdateType::AnyFixedGameUpdate),
		[this]() { UpdatePlayers(); }
	);
}

void PlayerManager::OnDestroyed()
{
	ClearPlayers();
}

void PlayerManager::UpdatePlayers()
{
	for(size_t i = 0; i < _players.size(); ++i)
	{
		if(_players[i] != nullptr)
		{
			_players[i]->Update();
		}
	}
}

PlayerManager::PlayerManager()
{
	_players.resize(VE_MAX_PLAYERS);
}