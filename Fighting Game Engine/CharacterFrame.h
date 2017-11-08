#pragma once
#include <vector>
#include <memory>
#include <string>
#include "JSON.h"

class AttackData;
class DefenceData;
class CollisionBox;
class CharacterSprite;

class CharacterFrame
{
	std::string _id;

	std::vector<AttackData> _hitboxes;
	std::vector<DefenceData> _hurtboxes;
	std::vector<CollisionBox> _collision;

	std::unique_ptr<CharacterSprite> _frameData;

public:

	const CharacterSprite* frameData() const;

	CharacterFrame(const json& j);
	~CharacterFrame();
};