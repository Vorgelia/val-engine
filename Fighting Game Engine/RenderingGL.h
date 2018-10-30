#pragma once
#include "BaseService.h"
#include <vector>
#include <GLM/glm.hpp>
#include "Font.h"
#include "ObjectReferenceManager.h"

class Mesh;
class BaseCamera;
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

class RenderingCommand;

class RenderingGL : public BaseService
{
	friend class GameSceneManager;
	friend class BaseCamera;

private:
	Debug* _debug{};
	GraphicsGL* _graphics{};
	ResourceManager* _resourceManager{};
	Screen* _screen{};

private:
	std::unordered_set<ObjectReference<BaseCamera>> _cameras;

	std::unique_ptr<FrameBuffer> _mainBuffer;

	std::vector<std::unique_ptr<FrameBuffer>> _temporaryFrameBuffers;
	std::unordered_set<FrameBuffer*> _reservedTemporaryFrameBuffers;

	std::vector<std::pair<ObjectReference<BaseCamera>, std::vector<RenderingCommand>>> _perCameraRenderingCommands;

	glm::mat4 _uiProjectionMatrix;

	std::unique_ptr<TimeDataBuffer> _timeDataBuffer;
	std::unique_ptr<RenderingDataBuffer> _renderingDataBuffer;
	std::unique_ptr<Vec4Buffer> _commonComputeVec4Buffer;

	void RegisterCamera(BaseCamera* camera);
	void UnregisterCamera(BaseCamera* camera);

	void ApplyRenderingCommandsForCamera(const BaseCamera* camera, const std::vector<RenderingCommand>& renderingCommands);

	void InitTextDrawing();
	void DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex) const;

	void BindMaterialUniforms(const Material& material, int& inout_textureUnitOffset) const;
	void BindShaderTextures(SurfaceShader* shader, const std::vector<MaterialTexture>& textures, int& inout_textureUnitOffset) const;
	void BindBufferUniforms(SurfaceShader* shad, int& inout_textureUnitOffset);

	void BindFrameBufferImages(const FrameBuffer* buffer, GLuint bindingPoint) const;

	void OnScreenResize();
	
	void BeginFrame();
	void EndFrame();

public:
	void OnInit() override;
	void OnDestroyed() override;

	FrameBuffer* GetTemporaryFrameBuffer();
	void ReleaseTemporaryFrameBuffer(FrameBuffer* frameBuffer);

	void AddRenderingCommandsForCamera(const BaseCamera* camera,  std::vector<RenderingCommand> renderingCommands);

	void ApplyMaterialToFrameBuffer(const FrameBuffer* frameBuffer, const Material* material);

	void ApplyRenderingCommands();

	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, const std::vector<MaterialTexture>& textures, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* frameBuffer, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));

	void DrawScreenText(glm::vec4 rect, GLuint size, const std::string& text, Font* font, TextAlignment alignment = TextAlignment::Left);

	const FrameBuffer* GetFramebuffer(int index = -1);

	RenderingGL() = default;
	~RenderingGL() = default;
};
