#pragma once
#include <memory>
#include <unordered_set>
#include <string>
#include "JSON.h"
#include "InputMotion.h"

class Script;
class InputMotionComponent;
class InputMotion;

class CharacterState
{
	std::string _name;
	int _priority;

	Script* _script;
	std::unordered_set<std::string> _stateTypeFlags;
	InputMotion _associatedMotion;

public:
	std::string name() const;
	int priority() const;

	Script* script();

	//air, crouching, light, medium, etc
	const std::unordered_set<std::string>& stateTypeFlags() const;
	const InputMotion& associatedMotion() const;

	CharacterState(const json& j, Script* script);
	~CharacterState();
};