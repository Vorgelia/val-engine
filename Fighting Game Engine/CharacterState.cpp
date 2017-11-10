#include "CharacterState.h"
#include "Script.h"
#include "InputMotion.h"
#include "ScriptManager.h"

std::string CharacterState::name()
{
	return _name;
}

int CharacterState::priority()
{
	return _priority;
}

Script * CharacterState::script()
{
	return _script;
}

const std::vector<std::string> CharacterState::stateTypeFlags()
{
	return _stateTypeFlags;
}

CharacterState::CharacterState(const json& j)
{
	_name = JSON::Get<std::string>(j["name"]);
	_priority = JSON::Get<int>(j["priority"]);
	_script = ScriptManager::GetScript(JSON::Get<std::string>(j["script"]));
	_stateTypeFlags.reserve(j["flags"].size());

	for(auto& iter : j["flags"])
	{
		_stateTypeFlags.push_back(JSON::Get<std::string>(iter));
	}

	_associatedMotion.reserve(j["motion"].size());
	for(auto& iter : j["motion"])
	{
		_associatedMotion.push_back(InputMotionComponent(iter));
	}
}

CharacterState::~CharacterState()
{

}
