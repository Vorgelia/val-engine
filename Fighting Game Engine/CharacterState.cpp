#include "CharacterState.h"
#include "Script.h"
#include "InputMotion.h"
#include "ScriptManager.h"

std::string CharacterState::name() const
{
	return _name;
}

int CharacterState::priority() const
{
	return _priority;
}

Script * CharacterState::script()
{
	return _script;
}

const std::unordered_set<std::string>& CharacterState::stateTypeFlags() const
{
	return _stateTypeFlags;
}

const InputMotion& CharacterState::associatedMotion() const
{
	return _associatedMotion;
}

CharacterState::CharacterState(const json& j, Script* script):
	_associatedMotion(j["motion"]),
	_script(script)
{
	JSON::TryGetMember<std::string>(j, "name", _name);
	JSON::TryGetMember<int>(j, "priority", _priority);

	_stateTypeFlags.reserve(j["flags"].size());
	for(auto& iter : j["flags"])
	{
		_stateTypeFlags.emplace(JSON::Get<std::string>(iter));
	}
}

CharacterState::~CharacterState()
{

}
