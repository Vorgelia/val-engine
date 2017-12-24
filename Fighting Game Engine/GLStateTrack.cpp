#include "GLStateTrack.h"
#include "ServiceManager.h"
#include "Profiler.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Material.h"
#include "DebugLog.h"
#include "Screen.h"
#include "InputManager.h"
#include <memory>

void GraphicsGL::Update() {}

std::unique_ptr<Texture> GraphicsGL::CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format, GLuint filt, GLuint edgeBehaviour)
{
	GLuint textureId;
	glGenTextures(1, &textureId);

	std::unique_ptr<Texture> texture = std::make_unique<Texture>(name, textureId, dimensions, format, filt, edgeBehaviour);

	BindTexture(*texture, 0);

	int dataType = format;
	switch(format)
	{
	case GL_RGBA16F:
	case GL_RGBA32F:
		dataType = GL_RGBA;
		break;
	}

	const unsigned char* data = (pixels.size() > 0) ? &pixels[0] : nullptr;
	glTexImage2D(GL_TEXTURE_2D, 0, format, dimensions.x, dimensions.y, 0, dataType, GL_UNSIGNED_BYTE, data);

	UpdateTexture(*texture, pixels);
	return texture;
}

void GraphicsGL::UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels)
{
	BindTexture(texture, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture._edgeBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture._edgeBehaviour);

	if(texture._filtering == GL_NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture._filtering);
}

void GraphicsGL::DestroyTexture(Texture& texture)
{
	glDeleteTextures(1, &texture._id);
	texture._valid = false;
}

bool GraphicsGL::ToggleFeature(GLenum feature, bool enable)
{
	if(glFeatures.find(feature) == glFeatures.end() || glFeatures[feature] != enable)
	{
		glFeatures[feature] = enable;
		if(enable)
			glEnable(feature);
		else
			glDisable(feature);

		return true;
	}
	return false;
}

bool GraphicsGL::BindFrameBufferId(GLuint id)
{
	if(boundFramebuffer != id)
	{
		boundFramebuffer = id;
		glBindFramebuffer(GL_FRAMEBUFFER, boundFramebuffer);
		return true;
	}

	return false;
}

void GraphicsGL::InitializeGL()
{	
	//Init GLFW
	glfwInit();
	//Window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef VE_USE_SINGLE_BUFFER
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
#endif

	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//Initialize screen 
	_screen = _serviceManager->Screen();

	//Initialize and store window
	_screen->window = glfwCreateWindow(_screen->size.x, _screen->size.y, "Videogame", nullptr, nullptr);
	glfwMakeContextCurrent(_screen->window);
	//Callbacks
	glfwSetWindowPos(_screen->window, (int)(_screen->mode->width*0.5 - _screen->size.x*0.5), (int)(_screen->mode->height*0.5 - _screen->size.y*0.5));
	glfwSetKeyCallback(_screen->window, InputManager::KeyCallback);
	glfwSetWindowSizeCallback(_screen->window, _screen->HandleResized);
	//Vsync
	glfwSwapInterval(0);

	glewExperimental = true;
	glewInit();
}

void GraphicsGL::CleanupGL()
{
	glfwTerminate();
}

std::unique_ptr<FrameBuffer> GraphicsGL::CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
{
	std::unique_ptr<FrameBuffer> frameBuffer = std::make_unique<FrameBuffer>(size, texAmount, depthStencil, format, clearColor, filtering, clearFlags);

	frameBuffer->textures.reserve(texAmount);
	for(int i = 0; i < texAmount; ++i)
	{
		frameBuffer->textures.push_back(CreateTexture("", std::vector<unsigned char>(), size, format, filtering, GL_CLAMP_TO_EDGE));
	}

	UpdateFrameBuffer(*frameBuffer);
	return frameBuffer;
}

void GraphicsGL::UpdateFrameBuffer(FrameBuffer& frameBuffer)
{
	frameBuffer.invResolution = glm::vec2(1.0f / frameBuffer.resolution.x, 1.0f / frameBuffer.resolution.y);

	std::vector<GLuint> attachments;
	if(!frameBuffer._valid)
	{
		glGenFramebuffers(1, &frameBuffer.id);
	}

	BindFrameBuffer(frameBuffer);

	for(unsigned int i = 0; i < frameBuffer.textures.size(); ++i)
	{
		BindTexture(*frameBuffer.textures[i], 0);

		if(!frameBuffer._valid)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBuffer.textures[i]->_id, 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
	}

	if(frameBuffer.hasDepthStencil)
	{
		if(!frameBuffer._valid)
		{
			glGenRenderbuffers(1, &frameBuffer.depthStencil);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer.depthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBuffer.resolution.x, frameBuffer.resolution.y);
		if(!frameBuffer._valid)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer.depthStencil);
	}

	if(!frameBuffer._valid)
		glDrawBuffers(attachments.size(), &attachments[0]);

	glClearColor(frameBuffer.clearColor.x, frameBuffer.clearColor.y, frameBuffer.clearColor.z, frameBuffer.clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_debug->VE_LOG("Framebuffer generation failed.", LogItem::Type::Warning);
	}
	else
	{
		frameBuffer._valid = true;
	}

	BindDefaultFrameBuffer();
}

void GraphicsGL::ClearFrameBuffer(FrameBuffer& frameBuffer)
{
	BindFrameBuffer(frameBuffer);
	glClearColor(frameBuffer.clearColor.x, frameBuffer.clearColor.y, frameBuffer.clearColor.z, frameBuffer.clearColor.w);
	glClear(frameBuffer.clearFlags);
}

void GraphicsGL::DestroyFrameBuffer(FrameBuffer& frameBuffer)
{
	for(const std::unique_ptr<Texture>& tex : frameBuffer.textures)
	{
		DestroyTexture(*tex);
	}

	if(frameBuffer.hasDepthStencil)
		glDeleteRenderbuffers(1, &frameBuffer.depthStencil);

	if(frameBuffer._valid)
		glDeleteFramebuffers(1, &frameBuffer.id);

	frameBuffer._valid = false;
}

bool GraphicsGL::BindTextureUnit(GLuint pos)
{
	if(activeTexture > boundTextures.size())
	{
		_debug->VE_LOG("Attempting to bind texture unit beyond hardware range. (" + std::to_string(pos) + ", max " + std::to_string(_maxTextureUnits) + ")", LogItem::Type::Error);
		return;
	}

	if(activeTexture != pos)
	{
		activeTexture = pos;
		glActiveTexture(GL_TEXTURE0 + pos);
		return true;
	}
	return false;
}

bool GraphicsGL::BindShader(const Shader& shader)
{
	if(boundShader != shader.id)
	{
		boundShader = shader.id;
		glUseProgram(shader.id);
		return true;
	}

	return false;
}

bool GraphicsGL::BindMesh(const Mesh& mesh)
{
	if(boundVAO != mesh.vao)
	{
		boundVAO = mesh.vao;
		glBindVertexArray(boundVAO);
		return true;
	}

	return false;
}

bool GraphicsGL::BindFrameBuffer(const FrameBuffer& frameBuffer)
{
	if(frameBuffer._valid)
	{
		return BindFrameBufferId(frameBuffer.id);
	}

	return false;
}

bool GraphicsGL::BindDefaultFrameBuffer()
{
	return BindFrameBufferId(0);
}

void GraphicsGL::ApplyMaterial(const Material& material)
{
	ApplyMaterialProperties(material);
}

void GraphicsGL::ApplyMaterialProperties(const Material & material)
{
	if(material.HasProperty(Material::Properties::Cull))
		ToggleFeature(GL_CULL_FACE, true);
	else
		ToggleFeature(GL_CULL_FACE, false);

	if(material.HasProperty(Material::Properties::Blend))
		ToggleFeature(GL_BLEND, true);
	else
		ToggleFeature(GL_BLEND, false);

	if(material.HasProperty(Material::Properties::ZWrite))
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	if(material.HasProperty(Material::Properties::ZTest))
		ToggleFeature(GL_DEPTH_TEST, true);
	else
		ToggleFeature(GL_DEPTH_TEST, false);

}

bool GraphicsGL::BindTexture(const Texture& texture)
{
	if(boundTextures.at(activeTexture) != texture._id)
	{
		boundTextures.at(activeTexture) = texture._id;
		glBindTexture(GL_TEXTURE_2D, texture._id);
		return true;
	}
	return false;
}

bool GraphicsGL::BindTexture(const Texture& texture, GLuint pos)
{
	if(boundTextures.at(pos) != texture._id)
	{
		BindTextureUnit(pos);
		return BindTexture(texture);
	}
	return false;
}

GraphicsGL::GraphicsGL(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
	boundTextures.resize(_maxTextureUnits, (GLuint)0);

	_debug = serviceManager->Debug();
}
