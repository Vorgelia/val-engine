#pragma once
#include "BaseService.h"
#include "JSON.h"
#include <boost\filesystem.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

namespace FS = boost::filesystem;

class Debug;
class ResourceManager;

class VertexAttribute;
class Shader;
class Texture;
class TextureData;
struct MaterialTexture;
class PostEffect;
class Material;
class CachedMesh;
class InputEvent;
class Object;
enum class InputButton;
enum class InputDirection;

class FilesystemManager : public BaseService
{
private:
	Debug* _debug;
	ResourceManager* _resource;

public:
	std::string LoadTextResource(int id, const std::string& type = "TEXT");
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type);

	std::string ReturnFile(const FS::path& dir);
	std::vector<std::string> ReturnFileLines(const FS::path& dir, bool removeWhitespace);

	void ApplyFunctionToFiles(const FS::path& dir, std::function<void(const FS::path&)> func);

	json LoadJsonResource(FS::path path);

	std::unique_ptr<Object> LoadObject(const FS::path& path);
	std::unique_ptr<CachedMesh> LoadMeshVM(const FS::path& path);
	std::unique_ptr<Material> LoadMaterial(const FS::path& path);
	std::unique_ptr<PostEffect> LoadPostEffect(const FS::path& path);

	void LoadTextureData(const FS::path & path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size);
	void LoadControlSettings(const FS::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt);
	void LoadObjects(const FS::path& path, std::vector<std::unique_ptr<Object>>& objects);

	bool SaveFile(const FS::path& dir, std::string& content, int flags = std::ios::out | std::ios::trunc);

	void Init() override;
	void Update() override;
	void Cleanup() override;

	FilesystemManager(ServiceManager* serviceManager);
	~FilesystemManager() = default;
};