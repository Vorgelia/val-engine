#pragma once
#include "GLIncludes.hpp"
#include <GLM\glm.hpp>
#include <vector>
#include <memory>

class Texture;

class FrameBuffer
{
	friend class GraphicsGL;

private:
	bool _valid;

public:
	std::vector<std::unique_ptr<Texture>> textures;

	glm::vec4 clearColor;
	GLuint format;
	GLuint filtering;

	GLuint id;
	GLuint depthStencil;

	glm::ivec2 resolution;
	glm::vec2 invResolution;

	bool hasDepthStencil;

	GLuint clearFlags;

	FrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags);
	~FrameBuffer() = default;
};
