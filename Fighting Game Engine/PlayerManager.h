#pragma once
#include "BaseService.h"
#include "Delegate.h"
#include "InputDeviceId.h"
#include <unordered_map>
#include <memory>

class GamePlayer;
enum class GamePlayerType;

class PlayerManager : public BaseService
{
private:
	std::vector<std::unique_ptr<GamePlayer>> _players;

public:
	typedef Delegate<GamePlayer*> PlayerEventHandler;
	PlayerEventHandler PlayerAdded;
	PlayerEventHandler PlayerRemoved;

	GamePlayer* AddPlayer(int id, int inputDeviceId);
	GamePlayer* AddNetworkPlayer(int id);
	GamePlayer* GetPlayer(int id);
	void RemovePlayer(int id);
	void ClearPlayers();

	void Init() override;
	void Update() override;
	void Cleanup() override;

	PlayerManager(ServiceManager* serviceManager);
	~PlayerManager();
};