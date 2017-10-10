#pragma once
#include"CommonUtilIncludes.hpp"
class FrameBuffer
{
	bool _valid;
public:
	std::vector<GLuint> textures;

	glm::vec4 clearColor;
	GLuint format;
	GLuint filtering;

	GLuint id;
	GLuint depthStencil;

	glm::ivec2 resolution;
	glm::vec2 invResolution;

	bool hasDepthStencil;

	GLuint clearFlags;

	bool valid();
	void Clear();
	void Destroy();
	void Update();
	bool Bind();

	FrameBuffer(glm::ivec2, int texAmount, bool depthStencil = true, GLint format = GL_RGBA, glm::vec4 clearColor = glm::vec4(0, 0, 0, 0), GLint filtering = GL_LINEAR, GLuint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	~FrameBuffer();
};
