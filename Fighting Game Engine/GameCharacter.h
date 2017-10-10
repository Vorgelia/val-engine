#pragma once
#include <string>
#include <map>
#include "Script.h"

class Object;
class GameCharacter
{
	std::string _currentStateId;
	int _currentFrame;

	std::map<std::string, std::shared_ptr<Script>> _stateLookup;
public:

	Object* characterObject;

	GameCharacter();
	~GameCharacter();
};