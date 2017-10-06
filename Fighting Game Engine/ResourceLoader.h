#pragma once
#include "CommonUtilIncludes.hpp"
class VertexAttribute;
class Shader;
class Texture;
class MaterialTexture;
class PostEffect;
class Material;
enum class InputButton;
enum class InputDirection;
class InputEvent;
class Object;

enum class ResourceError
{
	FileUnavailable,
	FileUnreadable
};

namespace ResourceLoader
{
	std::string LoadTextResource(int id, const std::string& type = "TEXT");
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type);
	std::string ReturnFile(const FS::path& dir);
	std::vector<std::string> ReturnFileLines(const FS::path& dir, bool removeWhitespace);
	bool SaveFile(const FS::path& dir, std::string content, int flags = std::ios::out | std::ios::trunc);
	void LoadMeshVM(const FS::path& path, std::vector<float> **verts, std::vector<GLuint> **elements, std::vector<VertexAttribute> *vertexFormat);
	void LoadMaterial(const FS::path& path, Shader** shader, unsigned char* properties, std::map<std::string, GLfloat>* uniformFloats, std::map<std::string, MaterialTexture>* uniformTextures, std::map<std::string, glm::vec4>* uniformVectors);
	void LoadPostEffect(const FS::path& path, std::vector<std::pair<int, Material* >>* elements, bool* cbBefore, bool* cbAfter, int* order);
	void LoadControlSettings(const FS::path& path, std::unordered_map<InputDirection, InputEvent>* dir, std::unordered_map<InputButton, InputEvent>* bt);
	void LoadObjects(const FS::path& path, std::map<int, Object*>* objects);
	std::string DecodeError(ResourceError error);
}