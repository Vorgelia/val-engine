#include "CharacterHitData.h"
#include "CollisionBox.h"

const std::vector<CollisionBox>& CharacterCollisionData::collision() const
{
	return _collision;
}

CharacterCollisionData::CharacterCollisionData(const json & j)
{
	for(auto& iter : j["collision"])
	{
		_collision.emplace_back(JSON::Get<CollisionBox>(iter));
	}
}