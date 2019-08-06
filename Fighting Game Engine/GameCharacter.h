#pragma once
#include "GameObject.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "CharacterCollisionResult.h"

class Script;
class BaseScriptVariable;
class CharacterState;
class CharacterFrame;
class CharacterStateComponent;
class CharacterPhysicsComponent;
class CharacterEventComponent;
class GamePlayer;
struct GameCharacterData;

class ScriptManager;
class FilesystemManager;
class ResourceManager;

class GameCharacter : public GameObject
{
	VE_OBJECT_DECLARATION(GameCharacter);

	friend class ScriptManager;
	friend class CharacterRenderer;
	friend class CharacterStateComponent;
	friend class FightingStageBehaviour;

private:
	ScriptManager* _scriptManager;
	FilesystemManager* _filesystem;
	ResourceManager* _resource;

private:
	bool _initialized;
	std::string _dataPath;
	std::unique_ptr<GameCharacterData> _characterData;

	GamePlayer* _playerOwner;

	Script* _characterScript;
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	bool _flipped;
	std::unordered_set<std::string> _systemFlags;

	CharacterStateComponent* _stateComponent;
	CharacterPhysicsComponent* _physicsComponent;
	CharacterEventComponent* _eventComponent;

	void CharacterInit();
	void CharacterUpdate();
	void CharacterLateUpdate();

	void UpdateSystemFlags();

public:
	const GameCharacterData* characterData() const { return _characterData.get(); }
	CharacterStateComponent* stateComponent() const { return _stateComponent; };
	CharacterPhysicsComponent* physicsComponent() const { return _physicsComponent; }
	CharacterEventComponent* eventComponent() const { return _eventComponent; }

	void OnInit() override;
	void OnDestroyed() override;

	void Deserialize(const json& j) override;

	void SetOwner(GamePlayer* owner);

	CharacterCollisionResult GenerateCollisions(const GameCharacter* other) const;

	GameCharacter();
	~GameCharacter() = default;
};

struct CharacterSortingPredicate
{
	bool operator()(const GameCharacter* lhs, const GameCharacter* rhs) const;
};