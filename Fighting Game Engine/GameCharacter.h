#pragma once
#include "Behaviour.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Object;
class Script;
class BaseScriptVariable;
class CharacterState;
class CharacterFrame;
class CharacterStateManager;
class CharacterPhysicsManager;
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
	friend class CharacterStateManager;

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

	std::unique_ptr<CharacterStateManager> _stateManager;
	std::unique_ptr<CharacterPhysicsManager> _physicsManager;

	void CharacterInit();
	void CharacterUpdate();

public:
	VE_BEHAVIOUR_NAME(GameCharacter);

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	const GameCharacterData* characterData() const;
	CharacterStateManager* stateManager() const;
	CharacterPhysicsManager* physicsManager() const;

	void SetOwner(GamePlayer* owner);

	GameCharacter(Object* owner, ServiceManager* serviceManager, const json& j);
	~GameCharacter();
};