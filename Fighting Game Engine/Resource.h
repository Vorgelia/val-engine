#pragma once
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

namespace Resource
{
	extern std::unordered_map<std::string, CachedMesh*> cachedMeshes;
	extern std::unordered_map<std::string, Mesh*> meshes;
	extern std::unordered_map<std::string, Texture*> textures;
	extern std::unordered_map<std::string, Material*> materials;
	extern std::unordered_map<std::string, PostEffect*> postEffects;

	//Base
	extern std::unordered_map<std::string, Mesh*> baseMeshes;
	extern std::unordered_map<std::string, Texture*> baseTextures;
	extern std::unordered_map<std::string, Material*> baseMaterials;
	extern std::unordered_map<std::string, Shader*> shaders;
	extern std::unordered_map<std::string, Font*> fonts;

	void Init();
	Mesh* GetMesh(FS::path path, bool editable = false);
	Texture* GetTexture(FS::path path);
	Shader* GetShader(std::string name);
	Material* GetMaterial(FS::path name);
	Material* CopyMaterial(Material* mat);
	PostEffect* GetPostEffect(FS::path path);
	Font* GetFont(FS::path path);

	void Update();
	void Unload();
	void Cleanup();
}

