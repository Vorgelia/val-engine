#include "Texture.h"
#include "GLStateTrack.h"
#include "DebugLog.h"
#include <SOIL\src\SOIL.h>

const glm::vec4& Texture::size()
{
	return _size;
}

Texture::Texture(std::string name, GLuint id, glm::ivec2 dim, int format, GLuint filtering, GLuint edgeBehaviour)
{
	_name = name;
	_id = id;
	_edgeBehaviour = edgeBehaviour;
	_valid = id > 0;
	_format = format;
	_size = glm::vec4(dim, 1.0f / (float)(dim.x), 1.0f / (float)(dim.y));
	_filtering = filtering;
}