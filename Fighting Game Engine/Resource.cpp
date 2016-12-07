#include "Resource.h"
#include "CachedMesh.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "Material.h"
#include "Texture.h"
#include "PostEffect.h"
#include "Font.h"
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
		std::cout << "Loading Font " << path.string() << std::endl;
	}
	return rf;
}

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
			return nullptr;
		rt = new Texture(path.string(), path, GL_RGBA, SOIL_LOAD_RGBA, GL_NEAREST, GL_REPEAT);
		if (path.parent_path().leaf().string() == "Base")
			baseTextures.insert(std::pair<std::string, Texture*>(path.string(), rt));
		else
			textures.insert(std::pair<std::string, Texture*>(path.string(), rt));
		std::cout << "Loading Texture " << path.string() << std::endl;
	}
	if (rt != nullptr)
		return rt;
	std::cout << "Returning null on Texture " << path.string() << std::endl;
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
			return nullptr;
		rp = new PostEffect(path.string());
		Resource::postEffects.insert(std::pair<std::string, PostEffect*>(path.string(), rp));
		std::cout << "Loading Post Effect " << path.string() << std::endl;
	}
	if (rp==nullptr)
		std::cout << "Returning null on Post Effect " << path.string() << std::endl;
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
			return nullptr;
		rs = new Shader(name, { ShaderAttachment(ResourceLoader::ReturnFile(name + ".vert"), GL_VERTEX_SHADER), ShaderAttachment(ResourceLoader::ReturnFile(name + ".frag"), GL_FRAGMENT_SHADER) });
		Resource::shaders.insert(std::pair<std::string, Shader*>(name, rs));
		std::cout << "Loading Shader " << name << std::endl;
	}
	if (rs->valid())
		return shaders[name];
	else{
		std::cout << "Returning null on Shader " << name << std::endl;
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
			return nullptr;
		rm = new Material(path.string());
		if (path.parent_path().leaf().string() == "Base")
			baseMaterials.insert(std::pair<std::string, Material*>(path.string(), rm));
		else
			materials.insert(std::pair<std::string, Material*>(path.string(), rm));
		std::cout << "Loading Material " << path.string() << std::endl;
	}
	if (rm==nullptr)
		std::cout << "Returning null on Material " << path.string() << std::endl;
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
			return nullptr;

		if (cachedMeshes.count(path.string()) == 0)
			cachedMeshes[path.string()] = new CachedMesh(path);
		std::cout << "Loading Mesh: " << path.string() << std::endl;
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
	std::cout << "Returning null on Mesh " << path.string() << std::endl;
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
		delete i->second;
	}
	cachedMeshes.clear();
	for (auto i = textures.begin(); i != textures.end(); ++i){
		delete i->second;
	}
	textures.clear();
	for (auto i = materials.begin(); i != materials.end(); ++i){
		delete i->second;
	}
	materials.clear();
}

void Resource::Cleanup(){
	for (auto i = baseMeshes.begin(); i != baseMeshes.end(); ++i){
		delete i->second;
	}
	baseMeshes.clear();
	Unload();
	for (auto i = baseMaterials.begin(); i != baseMaterials.end(); ++i){
		delete i->second;
	}
	baseMaterials.clear();
	for (auto i = shaders.begin(); i != shaders.end(); ++i){
		delete i->second;
	}
	shaders.clear();
	for (auto i = baseTextures.begin(); i != baseTextures.end(); ++i){
		delete i->second;
	}
	baseTextures.clear();
	for (auto i = fonts.begin(); i != fonts.end(); ++i){
		delete i->second;
	}
	fonts.clear();
	for (auto i = postEffects.begin(); i != postEffects.end(); ++i){
		delete i->second;
	}
	postEffects.clear();
}