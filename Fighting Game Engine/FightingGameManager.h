#pragma once
#include "BaseService.h"
#include "FightingGameState.h"
#include <vector>

class GameSceneManager;
class GameScene;
class GamePlayer;
class PlayerManager;

class FightingGameManager : public BaseService
{
private:
	GameSceneManager* _gameSceneManager;
	PlayerManager* _playerManager;

private:
	FightingGameState _currentState;
	std::vector<GamePlayer> _currentPlayers;

	void HandleSceneLoaded(const GameScene* scene);
	void ChangeState(FightingGameState state);

	void Init() override;
	void Update() override;
	void Cleanup() override;

public:
	FightingGameManager(ServiceManager* serviceManager);
	~FightingGameManager();
};