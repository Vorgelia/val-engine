#pragma once
#include "CommonUtilIncludes.hpp"

class CachedMesh;
class Mesh;
class Texture;
class Material;
class Shader;
class PostEffect;
class Font;

namespace Resource
{
	extern std::map<std::string, CachedMesh*> cachedMeshes;
	extern std::map<std::string, Mesh*> meshes;
	extern std::map<std::string, Texture*> textures;
	extern std::map<std::string, Material*> materials;
	extern std::map<std::string, PostEffect*> postEffects;
	//Base
	extern std::map<std::string, Mesh*> baseMeshes;
	extern std::map<std::string, Texture*> baseTextures;
	extern std::map<std::string, Material*> baseMaterials;
	extern std::map<std::string, Shader*> shaders;
	extern std::map<std::string, Font*> fonts;

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

