#include "Font.h"
#include "Texture.h"
#include "DebugLog.h"

FontCharacter::FontCharacter(GLuint atlasIndex, glm::vec4 textureParams, glm::ivec2 size, glm::ivec2 bearing, GLuint advance)
{
	this->textureParams = textureParams;
	this->size = size;
	this->bearing = bearing;
	this->advance = advance;
	this->atlasIndex = atlasIndex;
}

Font::Font(const std::string& name, unsigned int textSize, unsigned int atlasSize, unsigned int atlasPadding) :
	name(name),
	textSize(textSize),
	atlasSize(atlasSize),
	atlasPadding(atlasPadding)
{

}

const GLuint Font::height()
{
	return _height;
}

const GLuint Font::topBearing()
{
	return _topBearing;
}

Texture* Font::GetAtlas(int index)
{
	return _atlases.at(index).get();
}

const FontCharacter* Font::GetCharacter(GLubyte ch)
{
	auto& iter = _characters.find(ch);
	if(iter == _characters.end())
	{
		return nullptr;
	}
	return &(iter->second);
}