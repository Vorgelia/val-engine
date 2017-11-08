#include "CharacterHitData.h"
#include "CollisionBox.h"

const std::vector<CollisionBox>& CharacterHitData::collision()
{
	return _collision;
}

CharacterHitData::CharacterHitData(const json & j)
{
	for(auto& iter : j["collision"])
	{
		_collision.push_back(CollisionBox(JSON::Get<glm::ivec4>(iter)));
	}
}

CharacterHitData::~CharacterHitData()
{
}
