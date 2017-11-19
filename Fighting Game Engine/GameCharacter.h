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

class GameCharacter :
	public Behaviour
{
	friend class ScriptManager;
	friend class CharacterRenderer;

	std::string _dataPath;
	bool _initialized;

	//TODO: Player owner

	glm::vec2 _sizeMultiplier;
	bool _flipped;

	Script* _characterScript;
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	std::unique_ptr<CharacterStateManager> _stateManager;

	void HandleCharacterData(const json& j);

	void CharacterInit();
	void CharacterUpdate();

public:
	VE_BEHAVIOUR_NAME(GameCharacter);

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	CharacterStateManager* stateManager();
	glm::vec2 sizeMultiplier();
	bool flipped();

	GameCharacter(Object* owner, const json& j);
	~GameCharacter();
};