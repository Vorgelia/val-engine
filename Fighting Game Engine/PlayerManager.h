#pragma once
#include <unordered_map>
#include <memory>

class GamePlayer;

class PlayerManager
{
	static std::unordered_map<int, std::unique_ptr<GamePlayer>> _players;

public:
	static GamePlayer* GetPlayer(int playerId);
	static void Init();
};