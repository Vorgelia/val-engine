#pragma once
#include "FightingGameState.h"
#include "Delegate.h"
#include <unordered_map>
#include <unordered_set>
#include "BaseGameManager.h"
#include "ObjectReferenceManager.h"
#include "GameCharacter.h"

class GameCharacter;
class GameSceneManager;
class GameScene;
class GamePlayer;
class PlayerManager;
class FightingStageBehaviour;

class FightingGameManager : public BaseGameManager
{
	VE_OBJECT_DECLARATION(FightingGameManager, BaseGameManager);

	friend class ScriptManager;
private:
	GameSceneManager* _gameSceneManager;
	PlayerManager* _playerManager;

private:
	FightingGameState _currentState;

	std::unordered_set<ObjectReference<GameCharacter>> _characters;

	void HandleSceneLoaded(const GameScene* scene);
	void ChangeState(FightingGameState state);

	void OnInit() override;
	void OnDestroyed() override;

	void Update();

	ObjectReference<GameCharacter> AddCharacter(const std::string& path);

public:
	typedef Delegate<FightingGameState> FightingGameStateEventHandler;
	FightingGameStateEventHandler FightingGameStateChanged;

	FightingGameState currentState() const { return _currentState; }
	const std::unordered_set<ObjectReference<GameCharacter>>& characters() const { return _characters; }

	FightingGameManager() = default;
	~FightingGameManager() = default;
};