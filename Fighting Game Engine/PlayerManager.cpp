#include "PlayerManager.h"
#include "GamePlayer.h"

#define VE_MAX_PLAYERS 2
#define VE_MAX_SPECTATORS 14

std::unordered_map<int, std::unique_ptr<GamePlayer>> PlayerManager::_players;

GamePlayer* PlayerManager::GetPlayer(int playerId)
{
	auto& iter = _players.find(playerId);
	if(iter == _players.end())
	{
		return nullptr;
	}

	return iter->second.get();
}

void PlayerManager::Init()
{
	_players.reserve(VE_MAX_PLAYERS + VE_MAX_SPECTATORS);
}

void PlayerManager::Update()
{
	for(auto& iter : _players)
	{
		iter.second->Update();
	}
}
