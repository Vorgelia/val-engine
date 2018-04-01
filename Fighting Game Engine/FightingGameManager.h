#pragma once
#include "BaseService.h"
#include "FightingGameState.h"
#include "Delegate.h"

class GameSceneManager;
class GameScene;
class GamePlayer;
class PlayerManager;
class FightingStageBehaviour;

class FightingGameManager : public BaseService
{
private:
	GameSceneManager* _gameSceneManager;
	PlayerManager* _playerManager;

private:
	FightingGameState _currentState;

	FightingStageBehaviour* _stageBehaviour;

	void HandleSceneLoaded(const GameScene* scene);
	void ChangeState(FightingGameState state);

	void Init() override;
	void Update() override;
	void Cleanup() override;

public:
	typedef Delegate<FightingGameState> FightingGameStateEventHandler;
	FightingGameStateEventHandler FightingGameStateChanged;

	FightingGameState currentState() const;
	FightingStageBehaviour* stageBehaviour() const;

	FightingGameManager(ServiceManager* serviceManager);
	~FightingGameManager();
};