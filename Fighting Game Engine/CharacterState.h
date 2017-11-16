#pragma once
#include <memory>
#include <vector>
#include <string>
#include "JSON.h"

class Script;
class InputMotionComponent;

class CharacterState
{
	std::string _name;
	int _priority;

	Script* _script;
	std::vector<std::string> _stateTypeFlags;
	std::vector<InputMotionComponent> _associatedMotion;

public:
	std::string name() const;
	int priority() const;

	Script* script();
	//air, crouching, light, medium, etc
	const std::vector<std::string>& stateTypeFlags() const;
	const std::vector<InputMotionComponent>& associatedMotion() const;

	CharacterState(const json& j);
	~CharacterState();
};