#pragma once
#include "BaseService.h"
#include "FightingGameState.h"
#include "Delegate.h"
#include <unordered_map>
#include <unordered_set>

class GameCharacter;
class GameSceneManager;
class GameScene;
class GamePlayer;
class PlayerManager;
class FightingStageBehaviour;

class FightingGameManager : public BaseService
{
	friend class ScriptManager;
private:
	GameSceneManager* _gameSceneManager;
	PlayerManager* _playerManager;

private:
	FightingGameState _currentState;

	std::unordered_set<GameCharacter*> _characters;

	FightingStageBehaviour* _stageBehaviour;

	void HandleSceneLoaded(const GameScene* scene);
	void ChangeState(FightingGameState state);

	void Init() override;
	void Update() override;
	void Cleanup() override;

	GameCharacter* AddCharacter(const std::string& path);
	void RemoveCharacter(int id);
	void CleanupCharacters();

public:
	typedef Delegate<FightingGameState> FightingGameStateEventHandler;
	FightingGameStateEventHandler FightingGameStateChanged;

	FightingGameState currentState() const;
	FightingStageBehaviour* stageBehaviour() const;

	const std::unordered_set<GameCharacter*>& characters() const;

	void ChackCharacterCollisions(GameCharacter* other);

	FightingGameManager(GameInstance* serviceManager);
	~FightingGameManager() = default;
};