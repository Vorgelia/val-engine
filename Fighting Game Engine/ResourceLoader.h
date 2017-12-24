#pragma once
#include "JSON.h"
#include <boost\filesystem.hpp>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

namespace FS = boost::filesystem;

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

namespace ResourceLoader
{
	std::string LoadTextResource(int id, const std::string& type = "TEXT");
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type);

	std::string ReturnFile(const FS::path& dir);
	std::vector<std::string> ReturnFileLines(const FS::path& dir, bool removeWhitespace);

	void ApplyFunctionToFiles(const FS::path& dir, std::function<void(const FS::path&)> func);

	bool SaveFile(const FS::path& dir, std::string& content, int flags = std::ios::out | std::ios::trunc);

	void LoadTexture(const FS::path& path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size);
	void LoadMeshVM(const FS::path& path, std::vector<float>& out_verts, std::vector<GLuint>& out_elements, std::vector<VertexAttribute>& out_vertexFormat);
	void LoadMaterial(const FS::path& path, Shader*& shader, unsigned char& properties, std::unordered_map<std::string, GLfloat>& uniformFloats, std::unordered_map<std::string, MaterialTexture>& uniformTextures, std::unordered_map<std::string, glm::vec4>& uniformVectors);
	void LoadPostEffect(const FS::path& path, std::vector<std::pair<int, Material*>>& elements, bool& cbBefore, bool& cbAfter, int& order);
	void LoadControlSettings(const FS::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt);
	void LoadObjects(const FS::path& path, std::vector<std::unique_ptr<Object>>& objects);
	json LoadJsonResource(FS::path path);
	std::unique_ptr<Object> LoadObject(const FS::path& path);
}