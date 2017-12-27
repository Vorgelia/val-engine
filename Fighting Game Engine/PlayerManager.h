#pragma once
#include <unordered_map>
#include <memory>
#include "Delegate.h"

class GamePlayer;
enum class GamePlayerType;
//TODO: Make non-static
class PlayerManager
{
	static std::unordered_map<int, std::unique_ptr<GamePlayer>> _players;
	static std::unordered_map<int, std::unique_ptr<GamePlayer>> _spectators;

public:
	typedef Delegate<GamePlayer*> PlayerEventHandler;
	static PlayerEventHandler PlayerAdded;
	static PlayerEventHandler PlayerRemoved;

	static GamePlayer* AddPlayer(GamePlayerType type, int id);
	static GamePlayer* RemovePlayer(int id);
	static GamePlayer* GetPlayer(int id);


	static void Init();
	static void Update();

	PlayerManager() = delete;
};