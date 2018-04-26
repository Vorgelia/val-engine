#pragma once
#include <vector>
#include <memory>
#include <string>
#include "JSON.h"

struct CollisionBox;
class AttackData;
class DefenceData;
class CharacterSprite;

class CharacterFrame
{
	std::string _id;

	std::vector<AttackData> _hitboxes;
	std::vector<DefenceData> _hurtboxes;
	std::vector<CollisionBox> _collision;

	std::unique_ptr<CharacterSprite> _spriteData;

public:
	const std::vector<AttackData>& hitboxes() const { return _hitboxes; }
	const std::vector<DefenceData>& hurtboxes() const { return _hurtboxes; }
	const std::vector<CollisionBox>& collision() const { return _collision; }

	const CharacterSprite* spriteData() const;

	CharacterFrame(const json& j);
	~CharacterFrame();
};