#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Object;
class Script;

class GameCharacter
{
	std::string _currentStateId;
	Script* _currentState;

	int _currentFrame;

	std::unordered_map<std::string, std::unique_ptr<Script>> _stateLookup;
public:

	Object* characterObject;

	GameCharacter();
	~GameCharacter();
};