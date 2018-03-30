#include "GameCharacterData.h"

GameCharacterPhysicsParams::GameCharacterPhysicsParams(const json & j)
{
	JSON::TryGetMember(j, "gravity", gravity);
	JSON::TryGetMember(j, "baseGroundFriction", baseGroundFriction);
	JSON::TryGetMember(j, "baseGroundFriction", baseAirFriction);
}

GameCharacterData::GameCharacterData(const json& j)
{
	JSON::TryGetMember(j, "name", _name);
	JSON::TryGetMember(j, "flags", _flags);
	JSON::TryGetMember(j, "statePaths", _statePaths);
	JSON::TryGetMember(j, "frameDataPaths", _frameDataPaths);
	JSON::TryGetMember(j, "sizeMultiplier", _sizeMultiplier);
	JSON::TryGetMember(j, "characterScript", _characterScript);

	auto physicsData = j.find("physicsParams");
	if (physicsData != j.end())
	{
		_physicsParams = GameCharacterPhysicsParams(physicsData.value());
	}
}
