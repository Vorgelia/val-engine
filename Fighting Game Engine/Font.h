#pragma once
#include "CommonUtilIncludes.hpp"
#include<ft2build.h>
#include FT_FREETYPE_H

#define TEXT_SIZE 64
#define FONT_ATLAS_SIZE 1024
#define FONT_ATLAS_PADDING 2

class Texture;
enum class TextAlignment
{
	Left,
	Center,
	Right
};

class FontCharacter
{
public:
	glm::vec4 textureParams;
	GLuint atlasIndex;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;

	FontCharacter(GLuint atlasIndex, glm::vec4 textureParams, glm::ivec2 size, glm::ivec2 bearing, GLuint advance);
};

class Font
{
	std::unordered_map <GLubyte, FontCharacter*> characters;
public:
	std::string name;
	FT_Face face;

	GLint topBearing;
	GLuint height;
	static FT_Library library;
	static bool init;

	std::vector<Texture*> atlases;

	FontCharacter* Character(GLubyte ch);
	Font(const FS::path& p);
	~Font();
};

