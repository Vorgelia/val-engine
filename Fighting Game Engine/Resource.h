#pragma once
#include "BaseService.h"
#include <string>
#include <unordered_map>
#include <boost\filesystem.hpp>
namespace FS = boost::filesystem;

class CachedMesh;
class Mesh;
class Texture;
class Material;
class Shader;
class PostEffect;
class Font;

class Debug;
class GraphicsGL;
class FilesystemManager;

class ResourceManager : public BaseService
{
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	FilesystemManager * _filesystem;

private:
	std::unordered_map<std::string, std::unique_ptr<CachedMesh>> cachedMeshes;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
	std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
	std::unordered_map<std::string, std::unique_ptr<Material>> materials;
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
	std::unordered_map<std::string, std::unique_ptr<Font>> fonts;
	std::unordered_map<std::string, std::unique_ptr<PostEffect>> postEffects;

	//Base
	std::unordered_map<std::string, std::unique_ptr<Mesh>> baseMeshes;
	std::unordered_map<std::string, std::unique_ptr<Texture>> baseTextures;
	std::unordered_map<std::string, std::unique_ptr<Material>> baseMaterials;

	void GenerateDefaultTextures();
	void LoadDefaultResources();

public:
	Mesh* GetMesh(FS::path path);
	Texture* GetTexture(FS::path path);
	Shader* GetShader(std::string name);
	Material* GetMaterial(FS::path name);
	Material* CopyMaterial(Material* mat);
	PostEffect* GetPostEffect(FS::path path);
	Font* GetFont(FS::path path);

	void Init() override;
	void Update() override;
	
	void Unload();

	ResourceManager(ServiceManager* serviceManager);
	~ResourceManager();
};