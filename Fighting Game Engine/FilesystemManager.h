#pragma once
#include "BaseService.h"
#include "JSON.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace fs = std::filesystem;

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
enum class InputButton;
enum class InputDirection;

class FilesystemManager : public BaseService
{
private:
	Debug* _debug;
	ResourceManager* _resource;

public:
	std::string LoadTextResource(int id, const std::string& type = "TEXT") const;
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type) const;

	std::string ReturnFile(const FS::path& dir) const;
	std::vector<std::string> ReturnFileLines(const FS::path& dir, bool removeWhitespace = false) const;

	template<typename ResourceT>
	std::unique_ptr<ResourceT> LoadFileResource(const FS::path& path);

	void LoadTextureData(const FS::path & path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size) const;
	void LoadControlSettings(const FS::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt) const;

	void ApplyFunctionToFiles(const FS::path& dir, std::function<void(const FS::path&)> func) const;

	bool SaveFile(const FS::path& dir, std::string& content, int flags = std::ios::out | std::ios::trunc) const;

	void Init() override;
	void Update() override;
	void Cleanup() override;

	FilesystemManager(GameInstance* serviceManager);
	~FilesystemManager() = default;
};

template<>
std::unique_ptr<json> FilesystemManager::LoadFileResource(const FS::path& path);
template<>
std::unique_ptr<CachedMesh> FilesystemManager::LoadFileResource(const FS::path& path);
template<>
std::unique_ptr<Material> FilesystemManager::LoadFileResource(const FS::path& path);
template<>
std::unique_ptr<PostEffect>	FilesystemManager::LoadFileResource(const FS::path& path);

template<typename ResourceT>
inline std::unique_ptr<ResourceT> FilesystemManager::LoadFileResource(const FS::path & path)
{
	if(!FS::exists(path))
	{
		return std::unique_ptr<ResourceT>(nullptr);
	}
	return std::make_unique<ResourceT>(ReturnFile(path));
}
