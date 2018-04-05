#include "GameScene.h"
#include "Object.h"
#include "GameSceneManager.h"
#include "ServiceManager.h"
#include "FilesystemManager.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Behaviour.h"
#include "DebugLog.h"

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
		rfl = _filesystem->ReturnFileLines(_dataPath.string() + "/MaterialResources.txt", true);
		for (auto& i : rfl)
			if(!i.empty() && i.substr(0, 2) != "//")
				_resource->GetMaterial(i);//Materials also load their associated textures with them.

		rfl = _filesystem->ReturnFileLines(_dataPath.string() + "/MeshResources.txt", true);
		for (auto& i : rfl)
			if(!i.empty() && i.substr(0, 2) != "//")
				_resource->GetMesh(i);

		rfl = _filesystem->ReturnFileLines(_dataPath.string() + "/TextureResources.txt", true);
		for (auto& i : rfl)
			if(!i.empty() && i.substr(0, 2) != "//")
				_resource->GetTexture(i);

		json& j = *_resource->GetJsonData(_dataPath.string() + "/SceneObjects.txt");
		for(const json& objData : j)
		{
			std::string prefabPath;
			if(objData.size() == 1 && JSON::TryGetMember<std::string>(objData, "prefabPath", prefabPath))
			{
				AddObject(*_resource->GetJsonData(prefabPath));
			}
			else
			{
				AddObject(objData);
			}
			RegisterObject(_objects.back().get());
		}

		_postEffectsOrder = _filesystem->ReturnFileLines(_dataPath.string() + "/PostEffectsOrder.txt", true);
		for (auto& i : _postEffectsOrder)
			if(i.substr(0, 2) != "//" && !i.empty())
				_resource->GetPostEffect(i);
	}
	catch(std::runtime_error& err)
	{
		_debug->VE_LOG(err.what());
	}
	_loaded = true;
}

bool GameScene::loaded() const
{
	return _loaded;
}

const std::vector<std::string>& GameScene::postEffectsOrder() const
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

const std::string& GameScene::name() const
{
	return _name;
}

bool GameScene::initialized() const
{
	return _initialized;
}

void GameScene::Init()
{
	_initialized = true;
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(OnSceneInit));
}

void GameScene::EngineUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(EngineUpdate));
}

void GameScene::GameUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(GameUpdate));
}

void GameScene::LateGameUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(LateGameUpdate));
}

void GameScene::LateEngineUpdate()
{
	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(LateEngineUpdate));
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
		_rendering->DrawPostEffect(
			_resource->GetPostEffect(iter));
	}

	RunFunctionOnObjectBehaviours(VE_BEHAVIOUR_FUNCTION_CALLER(OnApplyPostEffects));
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

Object* GameScene::LoadObject(const std::string & prefabPath)
{
	const json* data = _resource->GetJsonData(prefabPath);
	if(data == nullptr)
	{
		return nullptr;
	}

	return AddObject(*data);
}

Object* GameScene::AddObject(const json & jsonData)
{
	int nearestAvailableId = _objects.empty() ? 0 : _objects.back()->_id + 1;
	while(_objectLookup.count(nearestAvailableId) > 0)
	{
		nearestAvailableId += 1;
	}

	_objects.emplace_back(
		std::make_unique<Object>(jsonData, _serviceManager, nearestAvailableId));
	Object* result = _objects.back().get();

	RegisterObject(result);

	return result;
}

void GameScene::DestroyObject(Object* object)
{
	for(auto& iter = _objects.begin();iter !=_objects.end();++iter)
	{
		if(iter->get() == object)
		{
			UnregisterObject(object);
			_objects.erase(iter);
			return;
		}
	}
}

void GameScene::DestroyObject(int objectId)
{
	auto iter = _objectLookup.find(objectId);
	if(iter == _objectLookup.end())
	{
		return;
	}

	DestroyObject(iter->second);
}

Object* GameScene::FindObject(const std::string& name)
{
	auto iter = _objectNameLookup.find(name);
	if(iter != _objectNameLookup.end())
	{
		return iter->second;
	}

	return nullptr;
}

Object* GameScene::FindObject(int id)
{
	auto iter = _objectLookup.find(id);
	if(iter != _objectLookup.end())
	{
		return iter->second;
	}
	return nullptr;
}

Behaviour* GameScene::FindBehaviour(const std::string& name)
{
	for(auto& obj : _objects)
	{
		Behaviour* behaviour = obj->GetBehaviour<Behaviour>(name);
		if(behaviour != nullptr)
		{
			return behaviour;
		}
	}

	return nullptr;
}

GameScene::GameScene(const FS::path& path, ServiceManager* serviceManager)
{
	_serviceManager = serviceManager;
	_debug = _serviceManager->Debug();
	_resource = _serviceManager->ResourceManager();
	_rendering = _serviceManager->Rendering();
	_filesystem = _serviceManager->Filesystem();

	_initialized = false;
	_loaded = false;
	_dataPath = path;
	_name = path.leaf().string();

	_postEffectsOrder.clear();
}

GameScene::~GameScene()
{
	GameScene::Cleanup();
}
