#pragma once
#include <memory>
#include <vector>
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
	std::vector<std::string> _stateTypeFlags;
	InputMotion _associatedMotion;

public:
	std::string name() const;
	int priority() const;

	Script* script();

	//air, crouching, light, medium, etc
	const std::vector<std::string>& stateTypeFlags() const;
	const InputMotion& associatedMotion() const;

	CharacterState(const json& j);
	~CharacterState();
};