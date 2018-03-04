#include "GameCharacterData.h"


GameCharacterData::GameCharacterData(const json& j)
{
	JSON::TryGetMember(j, "name", _name);
	JSON::TryGetMember(j, "flags", _flags);
	JSON::TryGetMember(j, "statePaths", _statePaths);
	JSON::TryGetMember(j, "frameDataPaths", _frameDataPaths);
	JSON::TryGetMember(j, "sizeMultiplier", _sizeMultiplier);
	JSON::TryGetMember(j, "characterScript", _characterScript);
}


GameCharacterData::~GameCharacterData()
{
}
