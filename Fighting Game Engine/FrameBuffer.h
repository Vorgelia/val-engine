#pragma once
#include "ValEngine.h"
#include "GLIncludes.hpp"
#include <GLM/glm.hpp>
#include <vector>
#include <memory>
#include "EnumUtils.h"

class Texture;

enum class FrameBufferFlags : std::uint8_t
{
	VE_BITMASK_VALUE_NONE,
	Valid = 0b0001,
	Dirty = 0b0010,
	DepthStencil = 0b0100,
	VE_BITMASK_VALUE_ALL,
};

VE_DECLARE_BITMASK_ENUM(FrameBufferFlags);

class FrameBuffer
{
	friend class GraphicsGL;
	friend class RenderingGL;

private:
	FrameBufferFlags _flags;

	glm::vec4 _clearColor;
	GLuint _format;
	GLuint _filtering;

	GLuint _id;
	GLuint _depthStencil;

	ve::ivec2 _resolution;
	glm::vec2 _invResolution;

	GLuint _clearFlags;

	std::vector<std::unique_ptr<Texture>> _textures;

public:
	FrameBufferFlags flags() const { return _flags; }
	ve::ivec2 resolution() const { return _resolution; }
	std::vector<Texture*> textures() const;

	void SetResolution(ve::ivec2 resolution);

	bool IsValid() const { return (_flags | FrameBufferFlags::Valid) != FrameBufferFlags::None; }
	bool IsDirty() const { return (_flags | FrameBufferFlags::Dirty) != FrameBufferFlags::None; }
	
	FrameBuffer(ve::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags);
	~FrameBuffer() = default;
};
