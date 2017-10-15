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

