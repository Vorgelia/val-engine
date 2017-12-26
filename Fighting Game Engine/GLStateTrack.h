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
class ShaderAttachment;
class Material;
class Font;

class Screen;
class Debug;

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

	std::unordered_map<GLenum, bool> _glFeatures;

	FT_Library _freetypeLibrary;

	bool ToggleFeature(GLenum feature, bool enable);
	bool BindFrameBufferId(GLuint id);

	GLuint CreateShaderAttachment(const ShaderAttachment& shaderAttachment);
	GLuint CreateShaderProgram(const std::vector<GLuint> shaders);

public:
	void Init() override;
	void Update() override;

	std::unique_ptr<Texture> CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format, GLuint filt, GLuint edgeBehaviour);
	void UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels);
	void DestroyTexture(Texture& texture);

	std::unique_ptr<FrameBuffer> CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil = true, GLint format = GL_RGBA, glm::vec4 clearColor = glm::vec4(0, 0, 0, 0), GLint filtering = GL_LINEAR, GLuint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	void UpdateFrameBuffer(FrameBuffer& frameBuffer);
	void ClearFrameBuffer(FrameBuffer& frameBuffer);
	void DestroyFrameBuffer(FrameBuffer& frameBuffer);

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
	bool BindDefaultFrameBuffer();

	void ApplyMaterial(const Material& material);
	void ApplyMaterialProperties(const Material& material);

	GraphicsGL(ServiceManager* serviceManager);
	~GraphicsGL();
};