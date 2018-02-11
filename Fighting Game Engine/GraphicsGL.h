#pragma once
#include "BaseService.h"
#include "GLIncludes.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <GLM\glm.hpp>

#include<ft2build.h>
#include FT_FREETYPE_H

class Shader;
class Mesh;
class CachedMesh;
class Texture;
class FrameBuffer;
class GraphicsBuffer;
class ShaderAttachment;
class Material;
class Font;

class Screen;
class Debug;

enum class ShaderProgramType;
enum class GraphicsBufferType;

class GraphicsGL : public BaseService
{
private:
	Debug* _debug;

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

	bool ToggleFeature(GLenum feature, bool enable);
	bool BindFrameBufferId(GLuint id);

	GLuint CreateShaderAttachment(const ShaderAttachment& shaderAttachment);
	GLuint CreateShaderProgram(const std::vector<GLuint>& shaders, ShaderProgramType type);

public:
	void Init() override;
	void Update() override;

	std::unique_ptr<Texture> CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format, GLuint filt, GLuint edgeBehaviour);
	void UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels);
	void DestroyTexture(Texture& texture);

	std::unique_ptr<FrameBuffer> CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil = true, GLint format = GL_RGBA16, glm::vec4 clearColor = glm::vec4(0, 0, 0, 0), GLint filtering = GL_LINEAR, GLuint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	void UpdateFrameBuffer(FrameBuffer& frameBuffer);
	void ClearFrameBuffer(FrameBuffer& frameBuffer);
	void DestroyFrameBuffer(FrameBuffer& frameBuffer);

	template<typename BufferT>
	std::unique_ptr<BufferT> CreateGraphicsBuffer(GLuint defaultSize, GraphicsBufferType type);
	void UpdateGraphicsBuffer(const GraphicsBuffer& buffer);

	std::unique_ptr<Font> CreateFont(std::string name);
	void DestroyFont(Font& font);

	std::unique_ptr<Shader> CreateShader(const std::string& name, const std::vector<ShaderAttachment>& attachments);
	void DestroyShader(Shader& shader);

	std::unique_ptr<Mesh> CreateMesh(const std::string& name, CachedMesh* meshData);
	void DestroyMesh(Mesh& mesh);

	bool BindTextureUnit(GLuint pos);
	bool BindShader(const Shader& id);
	bool BindMesh(const Mesh& mesh);
	bool BindTexture(const Texture& texture);
	bool BindTexture(const Texture& texture, GLuint pos);
	bool BindFrameBuffer(const FrameBuffer& frameBuffer);
	bool BindGraphicsBuffer(const GraphicsBuffer& buffer, GLenum target);
	bool BindDefaultFrameBuffer();

	void BindTextureToImageUnit(GLuint unit, const Texture& tex, GLenum accessType = GL_READ_WRITE);
	void BindBufferToBindingPoint(GLuint unit, const GraphicsBuffer& buffer);

	void ApplyMaterial(const Material& material);
	void ApplyMaterialProperties(const Material& material);

	void DispatchCompute(const Shader& shader, unsigned int workGroupSizeX = 1, unsigned int workGroupSizeY = 1, unsigned int workGroupSizeZ = 1);

	GraphicsGL(ServiceManager* serviceManager);
	~GraphicsGL();
};

template<typename BufferT>
inline std::unique_ptr<BufferT> GraphicsGL::CreateGraphicsBuffer(GLuint defaultSize, GraphicsBufferType type)
{
	GLuint bufferId;
	glGenBuffers(1, &bufferId);

	std::unique_ptr<BufferT> buffer = std::make_unique<BufferT>();
	buffer->_dataSize = defaultSize;
	buffer->_id = bufferId;
	buffer->_type = type;

	switch(type)
	{
	case GraphicsBufferType::ShaderStorage:
		BindGraphicsBuffer(*buffer, GL_SHADER_STORAGE_BUFFER);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * defaultSize, nullptr, GL_DYNAMIC_COPY);
		break;
		return nullptr;
	case GraphicsBufferType::Uniform:
		BindGraphicsBuffer(*buffer, GL_UNIFORM_BUFFER);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * defaultSize, nullptr, GL_DYNAMIC_DRAW);
		break;
	}

	return buffer;
}
