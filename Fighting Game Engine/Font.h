#pragma once
#include "GLIncludes.hpp"
#include <GLM/glm.hpp>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

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
	friend class GraphicsGL;
private:
	GLuint _height;
	GLuint _topBearing;

	std::unordered_map <GLubyte, FontCharacter> _characters;
	std::vector<Texture> _atlases;

public:
	const std::string name;

	const unsigned int textSize;
	const unsigned int atlasSize;
	const unsigned int atlasPadding;

	const GLuint height() const;
	const GLuint topBearing() const;

	const Texture& GetAtlas(int index);
	const FontCharacter* GetCharacter(GLubyte ch);

	Font(std::string name, unsigned int textSize = 64, unsigned int atlasSize = 1024, unsigned int atlasPadding = 2);
	~Font() = default;
};