#include "GameScene.h"
#include "Object.h"
#include "ResourceLoader.h"
#include "Rendering.h"
#include "Resource.h"
#include "Time.h"
#include "Screen.h"
#include "Behaviour.h"
#include "DebugLog.h"
#include "Object.h"

//GameScene variables are this engine's equivalent to levels. Why they were not named levels, who knows.
//The Unity style callbacks sort of betray my Unity background 
void GameScene::LoadResources()
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
			RegisterObject(iter.get());
		}

		_postEffectsOrder = ResourceLoader::ReturnFileLines(_dataPath.string() + "/PostEffectsOrder.txt", true);
		for(unsigned int i = 0; i < _postEffectsOrder.size(); ++i)
			if(_postEffectsOrder[i].substr(0, 2) != "//" && _postEffectsOrder[i] != "")
				Resource::GetPostEffect(_postEffectsOrder[i]);
	}
	catch(std::runtime_error err)
	{
		VE_DEBUG_LOG(err.what());
	}
	_loaded = true;
}

bool GameScene::loaded()
{
	return _loaded;
}

const std::vector<std::string>& GameScene::postEffectsOrder()
{
	return _postEffectsOrder;
}

void GameScene::RunFunctionOnObjectBehaviours(std::function<void(Behaviour*)> func)
{
	for(auto& iter : _objects)
	{
		if(iter != nullptr && iter->enabled)
		{
			iter->RunFunctionOnBehaviours(func);
		}
	}
}

void GameScene::RegisterObject(Object* obj)
{
	_objectNameLookup.insert(std::make_pair(obj->name(), obj));
	_objectLookup.insert(std::make_pair(obj->id(), obj));
}

void GameScene::UnregisterObject(Object* obj)
{
	_objectNameLookup.erase(obj->name());
	_objectLookup.erase(obj->id());
}

std::string GameScene::name()
{
	return _name;
}

bool GameScene::initialized()
{
	return _initialized;
}

void GameScene::Init()
{
	_initialized = true;
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(OnSceneInit));
}

void GameScene::Update()
{

}

void GameScene::GameUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(GameUpdate));
}

void GameScene::LateGameUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(LateGameUpdate));
}

void GameScene::LateUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(LateUpdate));
}

void GameScene::RenderUI()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(OnRenderUI));
}

void GameScene::OnLoaded()
{

}

void GameScene::RenderObjects()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(OnRenderObjects));
}

void GameScene::ApplyPostEffects()
{
	for(auto& iter : _postEffectsOrder)
	{
		Rendering::DrawPostEffect(
			Resource::GetPostEffect(iter));
	}
}

void GameScene::Cleanup()
{
	_objects.clear();
	_objectLookup.clear();
	_objectNameLookup.clear();

	_postEffectsOrder.clear();

	_loaded = false;
	_initialized = false;
}

Object * GameScene::AddObject(const std::string & prefabPath)
{
	_objects.emplace_back(ResourceLoader::LoadObject(prefabPath));
	Object* result = _objects.back().get();

	RegisterObject(result);
	
	return result;
}

Object* GameScene::FindObject(const std::string& name)
{
	auto& iter = _objectNameLookup.find(name);
	if(iter != _objectNameLookup.end())
	{
		return iter->second;
	}

	return nullptr;
}

GameScene::GameScene(const FS::path& path)
{
	_initialized = false;
	_loaded = false;
	this->_dataPath = path;
	_postEffectsOrder.clear();
}

GameScene::~GameScene()
{
	Cleanup();
}
