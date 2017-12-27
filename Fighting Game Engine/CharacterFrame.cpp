#include "CharacterFrame.h"
#include "AttackData.h"
#include "DefenceData.h"
#include "CollisionBox.h"
#include "CharacterSprite.h"

const CharacterSprite* CharacterFrame::spriteData() const
{
	return _spriteData.get();
}

CharacterFrame::CharacterFrame(const json& j)
{
	_hitboxes.reserve(j["hitboxes"].size());
	for(auto& iter : j["hitboxes"])
	{
		_hitboxes.push_back(AttackData(iter));
	}

	_hurtboxes.reserve(j["hurtboxes"].size());
	for(auto& iter : j["hurtboxes"])
	{
		_hurtboxes.push_back(DefenceData(iter));
	}

	_collision.reserve(j["collision"].size());
	for(auto& iter : j["collision"])
	{
		_collision.push_back(CollisionBox(JSON::Get<glm::ivec4>(iter)));
	}

	_spriteData = std::make_unique<CharacterSprite>(j["spriteData"]);
}

CharacterFrame::~CharacterFrame()
{

}
