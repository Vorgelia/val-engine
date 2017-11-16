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

enum class CharacterStateFlagType
{
	Generic = 0,
	Invuln = 1, 
};

class GameCharacter :
	public Behaviour
{
	friend class ScriptManager;

	std::string _dataPath;
	bool _initialized;

	Script* _characterScript;

	std::string _currentStateId;
	CharacterState* _currentState;
	CharacterFrame* _currentFrame;
	int _currentStateFrame;
	bool _stateEnded;

	//TODO: Player owner

	glm::vec2 _sizeMultiplier;
	bool _flipped;

	std::unordered_map<std::string, std::unique_ptr<CharacterState>> _stateLookup;
	std::unordered_map<std::string, std::unique_ptr<CharacterFrame>> _frameLookup;

	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	//Throw invuln, air invuln, etc
	std::unordered_map<CharacterStateFlagType, std::vector<std::string>> _flags;

	void HandleCharacterData(const json& j);

	void CharacterInit();
	void CharacterUpdate();
	void StateUpdate();

	void EvaluateNextState();

public:
	VE_BEHAVIOUR_NAME(GameCharacter);

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	const CharacterFrame* currentFrame();
	glm::vec2 sizeMultiplier();

	bool flipped();

	bool StartState(std::string name);
	bool SetFrame(std::string name);
	bool ModifyCurrentStateFrame(int newFrame);
	bool RestartState();
	void MarkStateEnded();

	GameCharacter(Object* owner, const json& j);
	~GameCharacter();
};