#pragma once
#include "BaseService.h"
#include "GLIncludes.hpp"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <GLM/glm.hpp>

#include<ft2build.h>
#include FT_FREETYPE_H

class Mesh;
class CachedMesh;
class Texture;
class FrameBuffer;
class GraphicsBuffer;
class ShaderAttachment;
class Material;
class Font;
class BaseShader;
class Screen;
class SurfaceShader;
class ComputeShader;
class Debug;

enum class ShaderProgramType;
enum class GraphicsBufferType;

class GraphicsGL : public BaseService
{
	VE_OBJECT_DECLARATION(GraphicsGL);

private:
	ObjectReference<Debug> _debug;

private:
	GLint _maxTextureUnits;

	GLuint _activeTexture;
	GLuint _boundShader;
	GLuint _boundVAO;
	GLuint _boundFramebuffer;

	std::vector<GLuint> _boundTextures;
	std::unordered_map<GLenum, GLuint> _boundBuffers;

	std::unordered_map<GLenum, bool> _glFeatures;

	FT_Library _freetypeLibrary;

	GLuint CombineShaderAttachments(const std::vector<ShaderAttachment>& attachments, ShaderProgramType type) const;

	bool ToggleFeature(GLenum feature, bool enable);
	bool BindFrameBufferId(GLuint id);
	bool BindGraphicsBufferId(GLuint id, GLenum target);

	GLuint CreateShaderAttachment(const ShaderAttachment& shaderAttachment) const;
	GLuint CreateShaderProgram(const std::vector<GLuint>& shaders, ShaderProgramType type) const;

public:
	void OnInit() override;
	void OnServiceInit() override;
	void OnDestroyed() override;

	Texture CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format = GL_RGBA16, GLuint filt = GL_NEAREST, GLuint edgeBehaviour = GL_REPEAT);
	void UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels);
	void DestroyTexture(Texture& texture) const;

	FrameBuffer CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil = true, GLint format = GL_RGBA16, glm::vec4 clearColor = glm::vec4(0, 0, 0, 0), GLint filtering = GL_LINEAR, GLuint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	void UpdateFrameBuffer(FrameBuffer& frameBuffer);
	void ClearFrameBuffer(FrameBuffer& frameBuffer);
	void DestroyFrameBuffer(FrameBuffer& frameBuffer) const;

	template<typename BufferT>
	BufferT CreateGraphicsBuffer(GLuint defaultSize, GraphicsBufferType type);
	void UpdateGraphicsBuffer(const GraphicsBuffer& buffer) const;

	Font CreateFont(std::string name);
	void DestroyFont(Font& font) const;

	template<typename ShaderT>
	ShaderT CreateShader(const std::string& name, const std::vector<ShaderAttachment>& attachments);
	void DestroyShader(BaseShader& shader) const;

	Mesh CreateMesh(const std::string& name, CachedMesh* meshData);
	void DestroyMesh(Mesh& mesh) const;

	bool BindTextureUnit(GLuint pos);
	bool BindShader(const BaseShader& shader);
	bool BindMesh(const Mesh& mesh);
	bool BindTexture(const Texture& texture);
	bool BindTexture(const Texture& texture, GLuint pos);
	bool BindFrameBuffer(const FrameBuffer& frameBuffer);
	bool BindGraphicsBuffer(const GraphicsBuffer& buffer, GLenum target);
	bool UnbindGraphicsBuffer(GLenum target);
	bool BindDefaultFrameBuffer();

	void BindTextureToImageUnit(GLuint unit, const Texture& tex, GLenum accessType = GL_READ_WRITE) const;
	void BindBufferToBindingPoint(GLuint unit, const GraphicsBuffer& buffer) const;

	void ApplyMaterial(const Material& material);
	void ApplyMaterialProperties(const Material& material);

	void DispatchCompute(const ComputeShader& shader, unsigned int workGroupSizeX = 1, unsigned int workGroupSizeY = 1, unsigned int workGroupSizeZ = 1);

	GraphicsGL();
	~GraphicsGL() = default;
};

template<typename BufferT>
inline BufferT GraphicsGL::CreateGraphicsBuffer(GLuint defaultSize, GraphicsBufferType type)
{
	GLuint bufferId;
	glGenBuffers(1, &bufferId);

	BufferT buffer = BufferT();
	buffer._dataSize = defaultSize;
	buffer._id = bufferId;
	buffer._type = type;

	switch(type)
	{
	case GraphicsBufferType::ShaderStorage:
		BindGraphicsBuffer(buffer, GL_SHADER_STORAGE_BUFFER);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * defaultSize, nullptr, GL_DYNAMIC_COPY);
		break;
	case GraphicsBufferType::Uniform:
		BindGraphicsBuffer(buffer, GL_UNIFORM_BUFFER);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * defaultSize, nullptr, GL_DYNAMIC_DRAW);
		break;
	}

	return buffer;
}

template<>
inline SurfaceShader GraphicsGL::CreateShader(const std::string & name, const std::vector<ShaderAttachment>& attachments)
{
	SurfaceShader shader = SurfaceShader(name, 0);
	shader._id = CombineShaderAttachments(attachments, shader._type);
	return shader;
}

template<>
inline ComputeShader GraphicsGL::CreateShader(const std::string & name, const std::vector<ShaderAttachment>& attachments)
{
	ComputeShader shader = ComputeShader(name, 0);
	shader._id = CombineShaderAttachments(attachments, shader._type);

	int workSize[3];
	glGetProgramiv(shader._id, GL_COMPUTE_WORK_GROUP_SIZE, workSize);
	shader._computeGroupSize = glm::ivec3(workSize[0], workSize[1], workSize[2]);

	return shader;
}
