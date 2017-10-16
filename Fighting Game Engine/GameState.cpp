#include "GameState.h"
#include "Object.h"
#include "ResourceLoader.h"
#include "Rendering.h"
#include "Resource.h"
#include "Time.h"
#include "Screen.h"
#include "DebugLog.h"
//GameState variables are this engine's equivalent to levels. Why they were not named levels, who knows.
//The Unity style callbacks sort of betray my Unity background 
void GameState::LoadResources()
{
	_loaded = false;
	_initialized = false;
	if(_dataPath.empty())
	{
		_loaded = true;
		return;
	}

	std::vector<std::string> rfl;
	try
	{
		rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/MaterialResources.txt", true);
		for(unsigned int i = 0; i < rfl.size(); ++i)
			if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
				Resource::GetMaterial(rfl[i]);//Materials also load their associated textures with them.

		rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/MeshResources.txt", true);
		for(unsigned int i = 0; i < rfl.size(); ++i)
			if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
				Resource::GetMesh(rfl[i]);

		rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/TextureResources.txt", true);
		for(unsigned int i = 0; i < rfl.size(); ++i)
			if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
				Resource::GetTexture(rfl[i]);

		ResourceLoader::LoadObjects(_dataPath.string() + "/SceneObjects.txt", _objects);
		for(auto& iter : _objects)
		{
			_objectLookup.emplace(std::make_pair(iter->id(), iter.get()));
			_objectNameLookup.emplace(std::make_pair(iter->name(), iter.get()));
		}

		_postEffectsOrder = ResourceLoader::ReturnFileLines(_dataPath.string() + "/PostEffectsOrder.txt", true);
		for(unsigned int i = 0; i < _postEffectsOrder.size(); ++i)
			if(_postEffectsOrder[i].substr(0, 2) != "//" && _postEffectsOrder[i] != "")
				Resource::GetPostEffect(_postEffectsOrder[i]);
	}
	catch(std::runtime_error err)
	{
		DebugLog::Push(err.what());
	}
	_loaded = true;
}

bool GameState::loaded()
{
	return _loaded;
}

const std::vector<std::string>& GameState::postEffectsOrder()
{
	return _postEffectsOrder;
}

bool GameState::initialized()
{
	return _initialized;
}

void GameState::Init()
{
	_initialized = true;
}

void GameState::Update()
{
	if(glfwGetKey(Screen::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
	}
}

void GameState::GameUpdate()
{

}

void GameState::FrameEnd()
{

}

void GameState::GUI()
{

}

void GameState::OnLoaded()
{

}

std::string GameState::Serialize()
{
	return "ERROR";
}

void GameState::Deserialize(const std::string& data)
{

}

void GameState::Cleanup()
{
	_objects.clear();
	_objectLookup.clear();
	_objectNameLookup.clear();

	_postEffectsOrder.clear();

	_loaded = false;
	_initialized = false;
}

void GameState::RenderObjects()
{
	for(auto& i : _objects)
	{
		i->Render();
	}
}

Object* GameState::FindObject(const std::string& name)
{
	auto& iter = _objectNameLookup.find(name);
	if(iter != _objectNameLookup.end())
	{
		return iter->second;
	}

	return nullptr;
}

GameState::GameState(const FS::path& path)
{
	_initialized = false;
	_loaded = false;
	this->_dataPath = path;
	_postEffectsOrder.clear();
}

GameState::~GameState()
{
	Cleanup();
}
