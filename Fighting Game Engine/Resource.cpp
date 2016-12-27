#include "Resource.h"
#include "CachedMesh.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "Material.h"
#include "Texture.h"
#include "PostEffect.h"
#include "Font.h"
#include "ResourceInitializer.h"
#include "DebugLog.h"
//If the following define exists, the engine will create some default resource files if the folder structure is missing.
//I opt to create and then read files rather than reading directly from the resources because the generated files have comments in them that explain how they work
//They are also easier to just modify instead of writing new ones from scratch
#define VE_CREATE_DEFAULT_RESOURCES

namespace Resource{
	std::map<std::string, CachedMesh*> cachedMeshes;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Texture*> textures;
	std::map<std::string, Material*> materials;
	
	//Base
	std::map<std::string, Mesh*> baseMeshes;
	std::map<std::string, Texture*> baseTextures;
	std::map<std::string, Material*> baseMaterials;
	std::map<std::string, PostEffect*> postEffects;
	std::map<std::string, Shader*> shaders;
	std::map<std::string, Font*> fonts;
}

void Resource::Init(){
	std::vector<float> pixels{
		1, 0, 1, 1,  0, 0, 0, 1,
		0, 0, 0, 1,  1, 0, 1, 1
	};
#ifdef VE_CREATE_DEFAULT_RESOURCES
	if (!FS::exists("Meshes/") || !FS::exists("Shaders/") || !FS::exists("Settings/") || !FS::exists("States/")){
		DebugLog::Push("-----\n\n\nFile structure invalid. Creating default resources.\n\n\n-----");
		ResourceInitializer::Init();
	}
#endif
	//Initialization of base resources. These will not be unloaded throughout the program, but they can be overriden per GameState.
	baseTextures.insert(std::pair<std::string, Texture*>("base_texture", new Texture("base_texture", pixels, glm::ivec2(2, 2), GL_RGBA, GL_NEAREST, GL_REPEAT)));
	pixels = { 0, 0, 0, 1 };
	baseTextures.insert(std::pair<std::string, Texture*>("black",new Texture("black",pixels,glm::ivec2(1,1),GL_RGBA,GL_NEAREST,GL_REPEAT)));
	pixels = { 1, 1, 1, 1 };
	baseTextures.insert(std::pair<std::string, Texture*>("white", new Texture("white", pixels, glm::ivec2(1, 1), GL_RGBA, GL_NEAREST, GL_REPEAT)));
	//Base shaders
	Resource::GetShader("Shaders/Base/2D");
	Resource::GetShader("Shaders/Base/Screen");
	//Base meshes
	Resource::GetMesh("Meshes/Base/quad.vm", true);
	Resource::GetMesh("Meshes/Base/screenQuad.vm", true);
	//Base materials
	Resource::GetMaterial("Materials/Base/Screen.vmat");
	//Fonts
	Resource::GetFont("Fonts/Amble.ttf");
}

void Resource::Update(){
	//?
}

Font* Resource::GetFont(FS::path path){
	Font* rf;
	if (fonts.count(path.string())){
		rf = fonts[path.string()];
	}
	else{
		if (!FS::exists(path))
			return nullptr;
		rf = new Font(path);
		fonts.insert(std::pair<std::string,Font*>(path.string(),rf));
		DebugLog::Push("Loading Font: "+path.string(),1);
	}
	return rf;
}
//The way these functions work is similar. If the resource exists in the per GameState resources, retrieve a pointer to it.
//Else, if the resource exists in the base resources, do the same.
//Else, attempt to load the resource, caching it as nullptr if it doesn't exist, so no further loading attempts are made.
Texture* Resource::GetTexture(FS::path path){
	Texture* rt;
	if (textures.count(path.string()) > 0){
		rt = textures[path.string()];
	}
	else if (baseTextures.count(path.string()) > 0){
		rt = baseTextures[path.string()];
	}
	else{
		if (!FS::exists(path))
			return textures[path.string()]=nullptr;
		rt = new Texture(path.string(), path, GL_RGBA, SOIL_LOAD_RGBA, GL_NEAREST, GL_REPEAT);
		if (path.parent_path().leaf().string() == "Base")
			baseTextures.insert(std::pair<std::string, Texture*>(path.string(), rt));
		else
			textures.insert(std::pair<std::string, Texture*>(path.string(), rt));
		DebugLog::Push("Loading Texture: " + path.string(), 1);
	}
	if (rt != nullptr)
		return rt;
	DebugLog::Push("Failed to load texture: " + path.string());
	return nullptr;
}

PostEffect* Resource::GetPostEffect(FS::path path){
	if (path.string() == "" || path.string() == "ERROR")
		return nullptr;

	PostEffect* rp;
	if (Resource::postEffects.count(path.string()) > 0)
		rp = Resource::postEffects[path.string()];
	else{
		if (!FS::exists(path))
			return Resource::postEffects[path.string()]=nullptr;
		rp = new PostEffect(path.string());
		Resource::postEffects.insert(std::pair<std::string, PostEffect*>(path.string(), rp));
		DebugLog::Push("Loading Post Effect: " + path.string(), 1);
	}
	if (rp==nullptr)
		DebugLog::Push("Failed to load Post Effect: "+path.string());
	return rp;
}

Shader* Resource::GetShader(std::string name){
	if (name == "" || name == "ERROR")
		return nullptr;

	Shader* rs;
	if (Resource::shaders.count(name) > 0)
		rs = Resource::shaders[name];
	else{
		if (!FS::exists(name+".vert")||!FS::exists(name+".frag"))
			return Resource::shaders[name]=nullptr;
		rs = new Shader(name, { ShaderAttachment(ResourceLoader::ReturnFile(name + ".vert"), GL_VERTEX_SHADER), ShaderAttachment(ResourceLoader::ReturnFile(name + ".frag"), GL_FRAGMENT_SHADER) });
		Resource::shaders.insert(std::pair<std::string, Shader*>(name, rs));
		DebugLog::Push("Loading Shader: " + name, 1);
	}
	if (rs->valid())
		return shaders[name];
	else{
		DebugLog::Push("Failed to loadShader: " + name);
		return nullptr;
	}
}

Material* Resource::GetMaterial(FS::path path, bool duplicate){
	if (path.string() == "" || path.string() == "ERROR")
		return nullptr;

	Material* rm;
	if (materials.count(path.string()) > 0){
		if (!duplicate){
			rm = materials[path.string()];
		}
		else{
			int copyIndex = 0;
			while (materials.count(path.string() + "_Copy" + std::to_string(copyIndex)) > 0){
				++copyIndex;
			}
			rm = new Material(*materials[path.string()]);
			rm->name = path.string() + "_Copy" + std::to_string(copyIndex);
			materials.insert(std::pair<std::string, Material*>(path.string() + "_Copy" + std::to_string(copyIndex), rm));
		}
	}
	else if (baseMaterials.count(path.string()) > 0){
		if (!duplicate){
			rm = baseMaterials[path.string()];
		}
		else{
			int copyIndex = 0;
			while (materials.count(path.string() + "_Copy" + std::to_string(copyIndex)) > 0){
				++copyIndex;
			}
			rm = new Material(*materials[path.string()]);
			rm->name = path.string() + "_Copy" + std::to_string(copyIndex);
			materials.insert(std::pair<std::string, Material*>(path.string() + "_Copy" + std::to_string(copyIndex), rm));
		}
	}
	else{
		if (!FS::exists(path))
			return materials[path.string()]=nullptr;
		rm = new Material(path.string());
		if (path.parent_path().leaf().string() == "Base")
			baseMaterials.insert(std::pair<std::string, Material*>(path.string(), rm));
		else
			materials.insert(std::pair<std::string, Material*>(path.string(), rm));
		DebugLog::Push("Loading Material: " + path.string(), 1);
	}
	if (rm==nullptr)
		DebugLog::Push("Failed to load material: " + path.string());
	return rm;
}

Mesh* Resource::GetMesh(FS::path path, bool editable){
	if (path.string() == "" || path.string() == "ERROR")
		return nullptr;

	Mesh* rm;
	if (meshes.count(path.string()) > 0){
		rm = meshes[path.string()];
	}
	else if (baseMeshes.count(path.string()) > 0){
		rm = baseMeshes[path.string()];
	}
	else{
		if (!FS::exists(path))
			return meshes[path.string()] = nullptr;

		if (cachedMeshes.count(path.string()) == 0)
			cachedMeshes[path.string()] = new CachedMesh(path);
		DebugLog::Push("Loading Mesh: " + path.string(), 1);
		rm = new Mesh(path.string(), cachedMeshes[path.string()], editable);
		if (path.parent_path().leaf().string() == "Base")
			meshes.insert(std::pair<std::string, Mesh*>(path.string(), rm));
		else
			baseMeshes.insert(std::pair<std::string, Mesh*>(path.string(), rm));
	}
	if (!rm->valid())
		delete rm;
	else if (rm != nullptr)
		return rm;
	DebugLog::Push("Failed to load Mesh: " + path.string(), 1);
	return nullptr;
}

void Resource::Unload(){
	for (auto i = meshes.begin(); i != meshes.end(); ++i){
		delete i->second;
	}
	meshes.clear();
	//Clear all meshes before clearing cached meshes.
	//Code is structured so that Meshes send messages to their cached meshes for memory management
	for (auto i = cachedMeshes.begin(); i != cachedMeshes.end(); ++i){
		if (i->second!=nullptr)
		delete i->second;
	}
	cachedMeshes.clear();
	for (auto i = textures.begin(); i != textures.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	textures.clear();
	for (auto i = materials.begin(); i != materials.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	materials.clear();
}

void Resource::Cleanup(){
	for (auto i = baseMeshes.begin(); i != baseMeshes.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	baseMeshes.clear();
	Unload();
	for (auto i = baseMaterials.begin(); i != baseMaterials.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	baseMaterials.clear();
	for (auto i = shaders.begin(); i != shaders.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	shaders.clear();
	for (auto i = baseTextures.begin(); i != baseTextures.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	baseTextures.clear();
	for (auto i = fonts.begin(); i != fonts.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	fonts.clear();
	for (auto i = postEffects.begin(); i != postEffects.end(); ++i){
		if (i->second != nullptr)
		delete i->second;
	}
	postEffects.clear();
}