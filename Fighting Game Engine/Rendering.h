#pragma once
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

//Controls whether a single render buffer should be used
#define VE_USE_SINGLE_BUFFER

namespace Rendering
{
	extern FrameBuffer* mainBuffer;
	extern std::vector<FrameBuffer*> auxBuffers;
	extern std::vector<Camera> cameras;
	extern glm::mat4 orthoMat;
	extern glm::mat4 screenMat;
	extern glm::vec4 tintColor;

	void Init();
	void BeginFrame();
	void EndFrame();
	void Cleanup();

	void DrawMesh(Transform* transform, Mesh* mesh, Material* mat, Camera* camera = nullptr);

	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat);
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, std::vector<std::pair<GLuint, glm::vec4>> textures, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, std::vector<Texture*> textures, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));
	void DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* textures, Material* mat, glm::vec4 params = glm::vec4(0, 0, 1, 1));

	void DrawPostEffect(PostEffect* pf);
	void DrawScreenText(glm::vec4 rect, GLuint size, std::string text, Font* font, TextAlignment alignment = TextAlignment::Left);

	void InitTextDrawing();
	void DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex);

	inline void BindBufferUniforms(Shader* shad, int& index);
	void BindEngineUniforms(Shader* shader);

	void OnScreenResize();
}
