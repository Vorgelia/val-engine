#pragma once
#include "BaseService.h"
#include "GLIncludes.hpp"
#include <vector>
#include <unordered_map>
#include <GLM\glm.hpp>

class Shader;
class Mesh;
class Texture;
class FrameBuffer;
class Material;

class Screen;
class Debug;

class GraphicsGL : public BaseService
{
private:
	Debug* _debug;
	Screen* _screen;

private:
	GLint _maxTextureUnits;

	GLuint activeTexture;
	GLuint boundShader;
	GLuint boundVAO;
	GLuint boundFramebuffer;

	std::vector<GLuint> boundTextures;

	std::unordered_map<GLenum, bool> glFeatures;

	bool ToggleFeature(GLenum feature, bool enable);
	bool BindFrameBufferId(GLuint id);

	void InitializeGL();
	void CleanupGL();

public:
	void Update();

	std::unique_ptr<Texture> CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format, GLuint filt, GLuint edgeBehaviour);
	void UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels);
	void DestroyTexture(Texture& texture);

	std::unique_ptr<FrameBuffer> CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil = true, GLint format = GL_RGBA, glm::vec4 clearColor = glm::vec4(0, 0, 0, 0), GLint filtering = GL_LINEAR, GLuint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	void UpdateFrameBuffer(FrameBuffer& frameBuffer);
	void ClearFrameBuffer(FrameBuffer& frameBuffer);
	void DestroyFrameBuffer(FrameBuffer& frameBuffer);

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
	~GraphicsGL() = default;
};