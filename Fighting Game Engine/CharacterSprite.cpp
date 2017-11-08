#include "CharacterSprite.h"


std::string CharacterSprite::sprite() const
{
	return _sprite;
}

glm::ivec4 CharacterSprite::pixelRect() const
{
	return _pixelRect;
}

glm::ivec2 CharacterSprite::originOffset() const
{
	return _originOffset;
}

CharacterSprite::CharacterSprite(const json& j)
{
	_sprite = JSON::Get<std::string>(j["sprite"]);
	_pixelRect = JSON::Get<glm::ivec4>(j["pixelRect"]);
	_originOffset = JSON::Get<glm::ivec2>(j["originOffset"]);
}

CharacterSprite::~CharacterSprite()
{

}
