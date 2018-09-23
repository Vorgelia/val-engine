#include "Texture.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

const glm::vec4& Texture::size() const
{
	return _size;
}

Texture::Texture(std::string name, GLuint id, glm::ivec2 dimensions, int format, GLuint filtering, GLuint edgeBehaviour)
{
	_name = std::move(name);
	_id = id;
	_edgeBehaviour = edgeBehaviour;
	_valid = id > 0;
	_format = format;
	_size = glm::vec4(dimensions, 1.0f / (float)(dimensions.x), 1.0f / (float)(dimensions.y));
	_filtering = filtering;
}