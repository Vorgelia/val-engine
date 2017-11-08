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
	std::string _currentStateId;
	CharacterState* _currentState;
	int _currentStateFrame;
	CharacterFrame* _currentFrame;

	Script* _characterScript;

	//TODO: Player owner

	glm::vec2 _sizeMultiplier;

	std::unordered_map<std::string, std::unique_ptr<CharacterState>> _stateLookup;
	std::unordered_map<std::string, std::unique_ptr<CharacterFrame>> _frameLookup;

	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	//Throw invuln, air invuln, etc
	std::unordered_map<CharacterStateFlagType, std::vector<std::string>> _flags;


public:
	VE_BEHAVIOUR_NAME(GameCharacter);

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	const CharacterFrame* currentFrame();
	glm::vec2 sizeMultiplier();

	GameCharacter(Object* owner, const json& j);
	~GameCharacter();
};