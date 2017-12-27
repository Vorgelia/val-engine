#include "FrameBuffer.h"
#include "DebugLog.h"
#include "GraphicsGL.h"
#include "Texture.h"

FrameBuffer::FrameBuffer(glm::ivec2 res, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
{
	resolution = res;
	invResolution = glm::vec2(1.0 / (double)res.x, 1.0 / (double)res.y);

	this->hasDepthStencil = depthStencil;
	this->format = format;
	this->clearColor = clearColor;
	this->filtering = filtering;
	this->clearFlags = clearFlags;

	_valid = false;
}