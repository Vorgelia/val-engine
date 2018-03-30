#include "CharacterHitData.h"
#include "CollisionBox.h"

const std::vector<CollisionBox>& CharacterHitData::collision() const
{
	return _collision;
}

CharacterHitData::CharacterHitData(const json & j)
{
	for(auto& iter : j["collision"])
	{
		_collision.emplace_back(JSON::Get<glm::ivec4>(iter));
	}
}

CharacterHitData::~CharacterHitData()
= default;
