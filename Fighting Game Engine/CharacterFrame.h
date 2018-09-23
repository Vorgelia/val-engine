#pragma once
#include <vector>
#include <memory>
#include <string>
#include "JSON.h"

struct CollisionBox;
class CharacterHitData;
class CharacterSprite;

class CharacterFrame
{
	std::string _id;

	std::vector<CharacterHitData> _hitboxes;
	std::vector<CharacterHitData> _hurtboxes;

	//TODO: + arbitrary named hit data boxes.
	std::vector<CollisionBox> _collision;

	std::unique_ptr<CharacterSprite> _spriteData;

public:
	const std::vector<CharacterHitData>& hitboxes() const { return _hitboxes; }
	const std::vector<CharacterHitData>& hurtboxes() const { return _hurtboxes; }
	const std::vector<CollisionBox>& collision() const { return _collision; }

	const CharacterSprite* spriteData() const;

	CharacterFrame(const json& j);
	~CharacterFrame();
};