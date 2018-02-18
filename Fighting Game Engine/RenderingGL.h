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
class GraphicsBuffer;
class PostEffect;

class BaseShader;
class ComputeShader;
class SurfaceShader;

struct MaterialTexture;

class Debug;
class GraphicsGL;
class ResourceManager;
class Screen;
class Time;

class TimeDataBuffer;
class RenderingDataBuffer;
class Vec4Buffer;

class RenderingGL : public BaseService
{
	friend class GameSceneManager;
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	ResourceManager* _resourceManager;
	Screen* _screen;
	Time* _time;

private:
	std::unique_ptr<FrameBuffer> _mainBuffer;
	std::vector<std::unique_ptr<FrameBuffer>> _auxBuffers;
	glm::mat4 _orthoMat;
	glm::mat4 _screenMat;

	std::unique_ptr<TimeDataBuffer> _timeDataBuffer;
	std::unique_ptr<RenderingDataBuffer> _renderingDataBuffer;
	std::unique_ptr<Vec4Buffer> _commonComputeVec4Buffer;

	void InitTextDrawing();
	void DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex);

	void BindMaterialUniforms(const Material& material, int& out_texturesBound);
	void BindShaderTextures(SurfaceShader* shader, const std::vector<MaterialTexture>& textures, int& out_textureUnitOffset);
	void BindBufferUniforms(SurfaceShader* shad, int& index);

	void BindFrameBufferImages(const FrameBuffer* buffer, GLuint bindingPoint);

	void OnScreenResize();
	
	void BeginFrame();
	void EndFrame();

public:
	std::vector<Camera> cameras;
	glm::vec4 tintColor;

	void DrawMesh(Transform* transform, Mesh* mesh, Material* mat, Camera* camera = nullptr);

	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, const std::vector<MaterialTexture>& textures, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* textures, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));

	void DrawPostEffect(PostEffect* pf);
	void DrawScreenText(glm::vec4 rect, GLuint size, std::string text, Font* font, TextAlignment alignment = TextAlignment::Left);

	const FrameBuffer* GetFramebuffer(int index = -1);

	void Init() override;
	void Update() override;

	RenderingGL(ServiceManager* serviceManager);
	~RenderingGL();
};
