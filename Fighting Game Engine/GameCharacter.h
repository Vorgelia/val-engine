#pragma once
#include <string>
#include <unordered_map>
#include "Script.h"

class Object;
class GameCharacter
{
	std::string _currentStateId;
	int _currentFrame;

	std::unordered_map<std::string, std::shared_ptr<Script>> _stateLookup;
public:

	Object* characterObject;

	GameCharacter();
	~GameCharacter();
};