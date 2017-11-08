#include "CharacterFrame.h"
#include "AttackData.h"
#include "DefenceData.h"
#include "CollisionBox.h"
#include "CharacterSprite.h"

const CharacterSprite * CharacterFrame::frameData() const
{
	return _frameData.get();
}

CharacterFrame::CharacterFrame(const json& j)
{

}

CharacterFrame::~CharacterFrame()
{

}
