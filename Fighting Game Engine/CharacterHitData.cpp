#include "CharacterHitData.h"

bool CharacterHitData::CanCollideWith_Impl(const CharacterHitData& other) const
{
	for(const std::string& str : _flagCollisionIgnoreMask)
	{
		if(other._flags.count(str) > 0)
		{
			return false;
		}
	}

	for(const std::string& str : _flagCollisionRequireMask)
	{
		if(other._flags.count(str) <= 0)
		{
			return false;
		}
	}

	return true;
}

void CharacterHitData::RegisterReflectionFields() const
{
	AddReflectionJsonField("data", _data);
}

bool CharacterHitData::CanCollideWith(const CharacterHitData & other) const
{
	return CanCollideWith_Impl(other) && other.CanCollideWith_Impl(*this);
}

CharacterHitData::CharacterHitData(const json& j) 
	: CharacterCollisionData(j)
	, _sequenceID(0)
{
	JSON::TryGetMember(j, "data", _data);
	JSON::TryGetMember(j, "sequenceID", _sequenceID);

	json flagsJson;
	if(JSON::TryGetMember(j, "flags", flagsJson))
	{
		for(auto& iter : flagsJson)
		{
			_flags.emplace(JSON::Get<std::string>(iter));
		}
	}	

	json flagCollisionMaskJson;
	if(JSON::TryGetMember(j, "flagCollisionRequireMask", flagCollisionMaskJson))
	{
		_flagCollisionRequireMask.reserve(flagCollisionMaskJson.size());
		for(auto& iter : flagCollisionMaskJson)
		{
			_flagCollisionRequireMask.emplace_back(JSON::Get<std::string>(iter));
		}
	}

	json flagCollisionIgnoreMaskJson;
	if(JSON::TryGetMember(j, "flagCollisionIgnoreMask", flagCollisionIgnoreMaskJson))
	{
		_flagCollisionIgnoreMask.reserve(flagCollisionIgnoreMaskJson.size());
		for(auto& iter : flagCollisionIgnoreMaskJson)
		{
			_flagCollisionIgnoreMask.emplace_back(JSON::Get<std::string>(iter));
		}
	}
}
