#pragma once
#include "Behaviour.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "CharacterCollisionResult.h"

class Object;
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

class GameCharacter :
	public Behaviour
{
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
	std::unique_ptr<GameCharacterData> _characterData;

	GamePlayer* _playerOwner;

	Script* _characterScript;
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	bool _flipped;
	std::unordered_set<std::string> _systemFlags;

	std::unique_ptr<CharacterStateComponent> _stateComponent;
	std::unique_ptr<CharacterPhysicsComponent> _physicsComponent;
	std::unique_ptr<CharacterEventComponent> _eventComponent;

	void CharacterInit();
	void CharacterUpdate();
	void CharacterLateUpdate();

	void UpdateSystemFlags();

public:
	VE_BEHAVIOUR_NAME(GameCharacter);

	const GameCharacterData* characterData() const;
	CharacterStateComponent* stateComponent() const;
	CharacterPhysicsComponent* physicsComponent() const;
	CharacterEventComponent* eventComponent() const;

	void SetOwner(GamePlayer* owner);

	CharacterCollisionResult GenerateCollisions(const GameCharacter* other) const;

	GameCharacter(Object* owner, ServiceManager* serviceManager, const json& j);
	~GameCharacter() = default;
};

struct CharacterSortingPredicate
{
	bool operator()(const GameCharacter* lhs, const GameCharacter* rhs) const;
};