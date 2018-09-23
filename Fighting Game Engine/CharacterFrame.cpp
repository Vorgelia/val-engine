#include "CharacterFrame.h"
#include "CharacterHitData.h"
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
		_hitboxes.emplace_back(iter);
	}

	_hurtboxes.reserve(j["hurtboxes"].size());
	for(auto& iter : j["hurtboxes"])
	{
		_hurtboxes.emplace_back(iter);
	}

	_collision.reserve(j["collision"].size());
	for(auto& iter : j["collision"])
	{
		_collision.emplace_back(JSON::Get<CollisionBox>(iter));
	}

	_spriteData = std::make_unique<CharacterSprite>(j["spriteData"]);
}

CharacterFrame::~CharacterFrame()
= default;
