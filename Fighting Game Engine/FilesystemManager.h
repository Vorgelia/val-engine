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
class Material;
class CachedMesh;
struct InputEvent;
enum class InputButton : uint16_t;
enum class InputDirection : uint8_t;

class FilesystemManager : public BaseService
{
	VE_OBJECT_DECLARATION(FilesystemManager, BaseService);

private:
	ObjectReference<Debug> _debug;
	ObjectReference<ResourceManager> _resource;

protected:
	template<typename ResourceT>
	ResourceT LoadFileResource(const fs::path& path);

public:
	std::string LoadTextResource(int id, const std::string& type = "TEXT") const;
	std::vector<unsigned char> LoadBinaryResource(int id, const std::string& type) const;

	std::string ReturnFile(const fs::path& dir) const;
	std::vector<std::string> ReturnFileLines(const fs::path& dir, bool removeWhitespace = false) const;

	template<typename ResourceT>
	ResourceT GetFileResource(const fs::path& path);

	void LoadTextureData(const fs::path & path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size) const;
	void LoadControlSettings(const fs::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt) const;

	void ApplyFunctionToFiles(const fs::path& dir, std::function<void(const fs::path&)> func) const;

	bool SaveFile(const fs::path& dir, std::string& content, int flags = std::ios::out | std::ios::trunc) const;

	virtual void OnInit() override;
	virtual void OnServiceInit() override;

	FilesystemManager() = default;
	~FilesystemManager() = default;
};

template<>
json FilesystemManager::LoadFileResource(const fs::path& path);
template<>
CachedMesh FilesystemManager::LoadFileResource(const fs::path& path);
template<>
Material FilesystemManager::LoadFileResource(const fs::path& path);

template <typename ResourceT>
ResourceT FilesystemManager::LoadFileResource(const fs::path& path)
{
	return ResourceT(ReturnFile(path));
}

template<typename ResourceT>
inline ResourceT FilesystemManager::GetFileResource(const fs::path & path)
{
	if(!fs::exists(path))
	{
		throw std::exception(("File " + path.string() +" Not Found.").c_str());
	}

	return LoadFileResource<ResourceT>(path);
}
