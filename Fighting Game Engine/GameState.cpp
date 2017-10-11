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
	if(!_dataPath.empty())
	{
		std::vector<std::string> rfl;

		//Load Materials
		try
		{
			rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/MaterialResources.txt", true);
			for(unsigned int i = 0; i < rfl.size(); ++i)
				if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
					Resource::GetMaterial(rfl[i]);//Materials also load their associated textures with them.
		}
		catch(ResourceError err)
		{
			DebugLog::Push(ResourceLoader::DecodeError(err) + "\n\t" + _dataPath.string() + "/MaterialResources.txt");
		}
		catch(...)
		{
			DebugLog::Push("Resource: Unidentified Exception when loading file \n\t" + _dataPath.string() + "/MaterialResources.txt");
		}

		//Load Meshes
		try
		{
			rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/MeshResources.txt", true);
			for(unsigned int i = 0; i < rfl.size(); ++i)
				if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
					Resource::GetMesh(rfl[i]);
		}
		catch(ResourceError err)
		{
			DebugLog::Push(ResourceLoader::DecodeError(err) + "\n\t" + _dataPath.string() + "/MeshResources.txt");
		}
		catch(...)
		{
			DebugLog::Push("Resource: Unidentified Exception when loading file \n\t" + _dataPath.string() + "/MeshResources.txt");
		}

		//Load Textures
		try
		{
			rfl = ResourceLoader::ReturnFileLines(_dataPath.string() + "/TextureResources.txt", true);
			for(unsigned int i = 0; i < rfl.size(); ++i)
				if(rfl[i].substr(0, 2) != "//" && rfl[i] != "")
					Resource::GetTexture(rfl[i]);
		}
		catch(ResourceError err)
		{
			DebugLog::Push(ResourceLoader::DecodeError(err) + "\n\t" + _dataPath.string() + "/TextureResources.txt");
		}
		catch(...)
		{
			DebugLog::Push("Resource: Unidentified Exception when loading file \n\t" + _dataPath.string() + "/TextureResources.txt");
		}

		//Load Scene Objects
		try
		{
			ResourceLoader::LoadObjects(_dataPath.string() + "/SceneObjects.txt", &_objects);
		}
		catch(ResourceError err)
		{
			DebugLog::Push(ResourceLoader::DecodeError(err) + "\n\t" + _dataPath.string() + "/SceneObjects.txt");
		}
		catch(...)
		{
			DebugLog::Push("Resource: Unidentified Exception when loading file \n\t" + _dataPath.string() + "/SceneObjects.txt");
		}

		//Load Post Effects
		try
		{
			try
			{
				_postEffectsOrder = ResourceLoader::ReturnFileLines(_dataPath.string() + "/PostEffectsOrder.txt", true);
				for(unsigned int i = 0; i < _postEffectsOrder.size(); ++i)
					if(_postEffectsOrder[i].substr(0, 2) != "//" && _postEffectsOrder[i] != "")
						Resource::GetPostEffect(_postEffectsOrder[i]);
			}
			catch(ResourceError err)
			{
				DebugLog::Push(ResourceLoader::DecodeError(err) + "\n\t" + _dataPath.string() + "/PostEffectsOrder.txt");
				throw;
			}
			catch(...)
			{
				DebugLog::Push("Resource: Unidentified Exception when loading file \n\t" + _dataPath.string() + "/PostEffectsOrder.txt");
				throw;
			}
		}
		catch(...)
		{
			_postEffectsOrder.clear();
		}

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
	for(unsigned int i = 0; i < _objects.size(); ++i)
		if(_objects[i] != nullptr)
			delete _objects[i];
	_objects.clear();
	_postEffectsOrder.clear();
	_loaded = false;
	_initialized = false;
}

void GameState::RenderObjects()
{
	for(auto i = _objects.begin(); i != _objects.end(); ++i)
		i->second->Render();
}

const Object* GameState::FindObject(const std::string& name)
{
	for(auto i = _objects.begin(); i != _objects.end(); ++i)
		if(i->second->name == name)
			return i->second;
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
