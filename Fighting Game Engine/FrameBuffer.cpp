#include "FrameBuffer.h"
#include "Texture.h"
#include "GraphicsGL.h"

FrameBuffer::FrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
{
	resolution = size;
	invResolution = glm::vec2(1.0 / (double)size.x, 1.0 / double(size.y));

	this->hasDepthStencil = depthStencil;
	this->format = format;
	this->_clearColor = clearColor;
	this->filtering = filtering;
	this->clearFlags = clearFlags;

	_valid = false;
}
