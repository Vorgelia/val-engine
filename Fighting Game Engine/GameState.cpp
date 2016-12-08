#include "GameState.h"
#include "Object.h"
#include "ResourceLoader.h"
#include "Rendering.h"
#include "Resource.h"
#include "Time.h"
#include "Screen.h"
//GameState variables are this engine's equivalent to levels. Why they were not named levels, who knows.
//The Unity style callbacks sort of betray my Unity background 
void GameState::LoadResources(){
	_loaded = false;
	_initialized = false;
	if (!dataPath.empty()){
		std::vector<std::string> rfl;

		rfl = ResourceLoader::ReturnFileLines(dataPath.string()+"/MaterialResources.txt", true);
		if (rfl.size()>0 && rfl[0] != "ERROR")
		for (unsigned int i = 0; i < rfl.size(); ++i)
			if (rfl[i].substr(0,2)!="//")
				Resource::GetMaterial(rfl[i],true);//Materials also load their associated textures with them.

		rfl = ResourceLoader::ReturnFileLines(dataPath.string() + "/MeshResources.txt", true);
		if (rfl.size()>0 && rfl[0] != "ERROR")
		for (unsigned int i = 0; i < rfl.size(); ++i)
			if (rfl[i].substr(0, 2) != "//")
				Resource::GetMesh(rfl[i]);

		rfl = ResourceLoader::ReturnFileLines(dataPath.string() + "/TextureResources.txt", true);
		if (rfl.size()>0 && rfl[0] != "ERROR")
		for (unsigned int i = 0; i < rfl.size(); ++i)
			if (rfl[i].substr(0, 2) != "//")
				Resource::GetTexture(rfl[i]);

		ResourceLoader::LoadObjects(dataPath.string() + "/SceneObjects.txt", &objects);


		postEffectsOrder = ResourceLoader::ReturnFileLines(dataPath.string() + "/PostEffectsOrder.txt", true);
		if (postEffectsOrder.size()>0 && postEffectsOrder[0] != "ERROR"){
			for (unsigned int i = 0; i < postEffectsOrder.size(); ++i)
				Resource::GetPostEffect(postEffectsOrder[i]);
		}
		else
			postEffectsOrder.clear();
	}
	_loaded = true;
}

bool GameState::loaded(){
	return _loaded;
}
bool GameState::initialized(){
	return _initialized;
}

void GameState::Init(){
	_initialized = true;
}

void GameState::Update(){
	if (glfwGetKey(Screen::window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
	}
}
void GameState::GameUpdate(){

}

void GameState::FrameEnd(){

}

void GameState::GUI(){

}

void GameState::OnLoaded(){

}

std::string GameState::Serialize(){
	return "ERROR";
}

void GameState::Deserialize(std::string data){

}

void GameState::Cleanup(){
	for (unsigned int i = 0; i < objects.size(); ++i)
		if (objects[i]!=nullptr)
			delete objects[i];
	objects.clear();
	postEffectsOrder.clear();
	_loaded = false;
	_initialized = false;
}

void GameState::RenderObjects(){
	for (auto i = objects.begin(); i != objects.end();++i)
		i->second->Render();
}

const Object* GameState::FindObject(std::string name){
	for (auto i = objects.begin(); i != objects.end(); ++i)
	if (i->second->name == name)
		return i->second;
	return nullptr;
}

GameState::GameState(FS::path path){
	_initialized = false;
	_loaded = false;
	this->dataPath = path;
	postEffectsOrder.clear();
}

GameState::~GameState(){
	Cleanup();
}
