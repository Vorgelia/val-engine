#pragma once
#include "BaseService.h"
#include "JSON.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
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
	VE_OBJECT_DECLARATION(FilesystemManager);

private:
	Debug* _debug;
	ResourceManager* _resource;

public:
	std::string LoadTextResource(int id, const std::string& type = "TEXT") const;
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type) const;

	std::string ReturnFile(const fs::path& dir) const;
	std::vector<std::string> ReturnFileLines(const fs::path& dir, bool removeWhitespace = false) const;

	template<typename ResourceT>
	std::unique_ptr<ResourceT> LoadFileResource(const fs::path& path);

	void LoadTextureData(const fs::path & path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size) const;
	void LoadControlSettings(const fs::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt) const;

	void ApplyFunctionToFiles(const fs::path& dir, std::function<void(const fs::path&)> func) const;

	bool SaveFile(const fs::path& dir, std::string& content, int flags = std::ios::out | std::ios::trunc) const;

	virtual void OnInit() override;

	FilesystemManager() = default;
	~FilesystemManager() = default;
};

template<>
std::unique_ptr<json> FilesystemManager::LoadFileResource(const fs::path& path);
template<>
std::unique_ptr<CachedMesh> FilesystemManager::LoadFileResource(const fs::path& path);
template<>
std::unique_ptr<Material> FilesystemManager::LoadFileResource(const fs::path& path);
template<>
std::unique_ptr<PostEffect>	FilesystemManager::LoadFileResource(const fs::path& path);

template<typename ResourceT>
inline std::unique_ptr<ResourceT> FilesystemManager::LoadFileResource(const fs::path & path)
{
	if(!fs::exists(path))
	{
		return std::unique_ptr<ResourceT>(nullptr);
	}
	return std::make_unique<ResourceT>(ReturnFile(path));
}
