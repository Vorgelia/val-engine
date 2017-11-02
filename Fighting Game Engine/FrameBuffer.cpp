#include "FrameBuffer.h"
#include "DebugLog.h"
#include "GLStateTrack.h"

bool FrameBuffer::valid()
{
	return _valid;

}
void FrameBuffer::Clear()
{
	GLState::BindFramebuffer(id);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(clearFlags);
}

bool FrameBuffer::Bind()
{
	if(_valid)
	{
		GLState::BindFramebuffer(id);
		return true;
	}

	VE_DEBUG_LOG("Failed to bind invalid framebuffer.");
	return false;
}

void FrameBuffer::Destroy()
{
	glDeleteTextures(textures.size(), &textures[0]);
	if(hasDepthStencil)
		glDeleteRenderbuffers(1, &depthStencil);

	if(_valid)
		glDeleteFramebuffers(1, &id);
	_valid = false;
}

void FrameBuffer::Update()
{
	invResolution = glm::vec2(1.0 / (double)resolution.x, 1.0 / (double)resolution.y);
	std::vector<GLuint> attachments;

	if(!_valid)
	{
		glGenFramebuffers(1, &id);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	for(unsigned int i = 0; i < textures.size(); ++i)
	{
		if(!_valid)
			glGenTextures(1, &textures[i]);
		GLState::BindTexture(textures[i], 0);

		//Format specs
		if(format == GL_RGBA || format == GL_RGBA16F || format == GL_RGBA32F)
			glTexImage2D(GL_TEXTURE_2D, 0, format, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, format, resolution.x, resolution.y, 0, format, GL_FLOAT, NULL);

		//Filtering specs
		if(!_valid)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		if(!_valid)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
	}

	if(hasDepthStencil)
	{
		if(!_valid)
			glGenRenderbuffers(1, &depthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
		if(!_valid)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencil);
		//attachments.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
	}

	if(!_valid)
		glDrawBuffers(attachments.size(), &attachments[0]);

	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		VE_DEBUG_LOG("Framebuffer generation failed.", LogItem::Type::Warning);
	}
	else
	{
		_valid = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(glm::ivec2 res, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
{
	resolution = res;
	invResolution = glm::vec2(1.0 / (double)res.x, 1.0 / (double)res.y);
	textures.resize(texAmount);

	this->hasDepthStencil = depthStencil;
	this->format = format;
	this->clearColor = clearColor;
	this->filtering = filtering;
	this->clearFlags = clearFlags;

	_valid = false;

	Update();
}

FrameBuffer::~FrameBuffer()
{
	Destroy();
}
