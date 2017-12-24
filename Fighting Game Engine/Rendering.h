#pragma once
#include "BaseService.h"
#include <vector>
#include <GLM\glm.hpp>
#include "Font.h"

class Mesh;
class Camera;
class Material;
class Texture;
class Transform;
class FrameBuffer;
class Shader;
class PostEffect;

struct MaterialTexture;

class Debug;
class GraphicsGL;
class ResourceManager;

//Controls whether a single render buffer should be used
#define VE_USE_SINGLE_BUFFER

class RenderingGL : public BaseService
{
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	ResourceManager*_resourceManager;

private:
	FrameBuffer* mainBuffer;
	std::vector<FrameBuffer*> auxBuffers;
	std::vector<Camera> cameras;
	glm::mat4 orthoMat;
	glm::mat4 screenMat;
	glm::vec4 tintColor;



	void InitTextDrawing();
	void DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex);

	void BindMaterialUniforms(const Material& material, int& out_texturesBound);
	void BindShaderTextures(Shader* shader, const std::vector<MaterialTexture>& textures, int& out_textureUnitOffset);
	void BindBufferUniforms(Shader* shad, int& index);
	void BindEngineUniforms(Shader* shader);

	void OnScreenResize();

public:
	void BeginFrame();
	void EndFrame();

	void DrawMesh(Transform* transform, Mesh* mesh, Material* mat, Camera* camera = nullptr);

	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, const std::vector<MaterialTexture>& textures, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* textures, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));

	void DrawPostEffect(PostEffect* pf);
	void DrawScreenText(glm::vec4 rect, GLuint size, std::string text, Font* font, TextAlignment alignment = TextAlignment::Left);


	RenderingGL(ServiceManager* serviceManager);
	~RenderingGL();
};
