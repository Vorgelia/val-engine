#include "FrameBuffer.h"
#include "Texture.h"
#include "GraphicsGL.h"

void FrameBuffer::SetResolution(ve::ivec2 resolution)
{
	_resolution = resolution;
	_invResolution = glm::vec2(1.0 / double(resolution.x), 1.0 / double(resolution.y));
	_flags |= FrameBufferFlags::Dirty;
}

FrameBuffer::FrameBuffer(ve::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
	: _flags(FrameBufferFlags::None)
	, _clearColor(clearColor)
	, _format(format)
	, _filtering(filtering)
	, _id(0)
	, _depthStencil(0)
	, _clearFlags(clearFlags)
{
	SetResolution(size);
	if(depthStencil)
	{
		_flags |= FrameBufferFlags::DepthStencil;
	}
}
