#include "CharacterState.h"
#include "Script.h"
#include "InputMotion.h"

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
	return _script.get();
}

const std::vector<std::string> CharacterState::stateTypeFlags()
{
	return _stateTypeFlags;
}

CharacterState::CharacterState(const json& j)
{

}

CharacterState::~CharacterState()
{

}
