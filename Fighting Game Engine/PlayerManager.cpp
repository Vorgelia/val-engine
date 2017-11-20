#include "PlayerManager.h"
#include "GamePlayer.h"

#define VE_MAX_PLAYERS 2

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
	_players.reserve(VE_MAX_PLAYERS);
}
