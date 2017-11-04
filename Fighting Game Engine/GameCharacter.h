#pragma once
#include "Behaviour.h"
#include <string>
#include <unordered_map>
#include <memory>

class Object;
class Script;
class CharacterFrame;

class GameCharacter :
	public Behaviour
{
	std::string _currentStateId;
	Script* _currentState;

	std::unordered_map<std::string, std::unique_ptr<Script>> _stateLookup;
	std::unordered_map<std::string, std::unique_ptr<CharacterFrame>> _characterFrames;

	CharacterFrame* _currentFrame;

public:

	GameCharacter(Object* obj, const json& j);
	~GameCharacter();
};