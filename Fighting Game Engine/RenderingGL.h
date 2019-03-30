#pragma once
#include "BaseService.h"
#include <vector>
#include <GLM/glm.hpp>
#include "Font.h"
#include "ObjectReferenceManager.h"
#include "Camera.h"
#include "TimeDataBuffer.h"
#include "RenderingDataBuffer.h"
#include "Vec4Buffer.h"
#include "FrameBuffer.h"

class Mesh;
class BaseCamera;
class Material;
class Texture;
class FrameBuffer;
class GraphicsBuffer;

class BaseShader;
class ComputeShader;
class SurfaceShader;

struct Transform;
struct MaterialTexture;

class Debug;
class GraphicsGL;
class ResourceManager;
class ScreenManager;
class Time;

class RenderingCommand;

class RenderingGL : public BaseService
{
	friend class GameSceneManager;
	friend class BaseCamera;

	VE_OBJECT_DECLARATION(RenderingGL);

private:
	ObjectReference<Debug> _debug{};
	ObjectReference<GraphicsGL> _graphics{};
	ObjectReference<ResourceManager> _resourceManager{};
	ObjectReference<ScreenManager> _screen{};

private:
	std::unordered_set<ObjectReference<BaseCamera>> _cameras;

	FrameBuffer _mainBuffer;

	std::vector<FrameBuffer> _temporaryFrameBuffers;
	std::unordered_set<FrameBuffer*> _reservedTemporaryFrameBuffers;

	glm::mat4 _uiProjectionMatrix;

	TimeDataBuffer _timeDataBuffer;
	RenderingDataBuffer _renderingDataBuffer;
	Vec4Buffer _commonComputeVec4Buffer;

	void RegisterCamera(BaseCamera* camera);
	void UnregisterCamera(BaseCamera* camera);

	void InitTextDrawing();
	void DrawTextCharacter(glm::vec4 rect, glm::vec4 params, const Texture& tex) const;

	void BindMaterialUniforms(const Material& material, int& inout_textureUnitOffset) const;
	void BindShaderTextures(SurfaceShader* shader, const std::vector<MaterialTexture>& textures, int& inout_textureUnitOffset) const;
	void BindBufferUniforms(SurfaceShader* shad, int& inout_textureUnitOffset);

	void BindFrameBufferImages(const FrameBuffer&buffer, GLuint bindingPoint) const;

	void OnScreenResize();
	
	void BeginFrame();
	void EndFrame();

public:
	const FrameBuffer& mainBuffer() const { return _mainBuffer; }

	void OnInit() override;
	void OnServiceInit() override;
	void OnDestroyed() override;

	FrameBuffer* GetTemporaryFrameBuffer();
	void ReleaseTemporaryFrameBuffer(FrameBuffer* frameBuffer);

	void RenderCamera(const BaseCamera* camera);
	void RenderAllCameras();

	void ApplyMaterialToFrameBuffer(const FrameBuffer* frameBuffer, const Material* material);

	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, const std::vector<MaterialTexture>& textures, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* frameBuffer, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));

	void DrawScreenText(glm::vec4 rect, GLuint size, const std::string& text, Font* font, TextAlignment alignment = TextAlignment::Left);

	RenderingGL() = default;
	~RenderingGL() = default;
};
