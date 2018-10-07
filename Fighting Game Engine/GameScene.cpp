#include "GameScene.h"
#include "GameSceneManager.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "DebugLog.h"

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
	}
	catch(std::runtime_error& err)
	{
		_debug->VE_LOG(err.what());
	}
	_loaded = true;
}

void GameScene::OnInit()
{
	_debug = _owningInstance->Debug();
	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();
	_filesystem = _owningInstance->Filesystem();
}

void GameScene::OnDestroyed()
{
	_objects.clear();
	_objectLookup.clear();
	_objectNameLookup.clear();

	_postEffectsOrder.clear();

	_loaded = false;
	_initialized = false;
}

bool GameScene::loaded() const
{
	return _loaded;
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

void GameScene::SortObjects()
{
	const auto sortingPredicate = [](const std::unique_ptr<Object>& lhs, const std::unique_ptr<Object>& rhs)->bool
	{
		return lhs->updatePriority > rhs->updatePriority;
	};

	std::sort(_objects.begin(), _objects.end(), sortingPredicate);
	_shouldSortObjects = false;
}

const std::string& GameScene::name() const
{
	return _name;
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
	_shouldSortObjects = true;

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
			_shouldSortObjects = true;
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

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	GameScene::Cleanup();
}
