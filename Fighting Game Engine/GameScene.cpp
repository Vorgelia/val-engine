#include "GameScene.h"
#include "GameSceneManager.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "DebugLog.h"

void GameScene::LoadResources()
{
	_loaded = false;
	if(_dataPath.empty())
	{
		_loaded = true;
		return;
	}

	std::vector<std::string> rfl;
	try
	{
		json& j = *_resource->GetJsonData(_dataPath.string());
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
	BaseObject::OnInit();
	_debug = _owningInstance->Debug();
	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();

	_timeTracker.Reset(_owningInstance->timeTracker().time());

	LoadResources();
}

void GameScene::OnDestroyed()
{
	_objects.clear();
	_objectNameLookup.clear();

	_loaded = false;
}

bool GameScene::loaded() const
{
	return _loaded;
}

void GameScene::RegisterObject(GameObject* obj)
{
	_objectNameLookup.insert(std::make_pair(obj->name(), obj));
}

void GameScene::UnregisterObject(GameObject* obj)
{
	_objectNameLookup.erase(obj->name());
}

const std::string& GameScene::name() const
{
	return _name;
}

ObjectReference<GameObject> GameScene::LoadObject(const std::string& prefabPath)
{
	const json* data = _resource->GetJsonData(prefabPath);
	if(data == nullptr)
	{
		return nullptr;
	}

	_objects.push_back(ObjectFactory::CreateObjectFromJson<GameObject>(this, *data));
	GameObject* result = _objects.back().get();
	RegisterObject(result);

	return ObjectReference<GameObject>(result);
}

void GameScene::DestroyObject(GameObject* object)
{
	for(auto& iter = _objects.begin(); iter != _objects.end(); ++iter)
	{
		if(iter->get() == object)
		{
			UnregisterObject(object);
			_objects.erase(iter);
			return;
		}
	}
}

GameObject* GameScene::FindObject(const std::string& name)
{
	auto iter = _objectNameLookup.find(name);
	if(iter != _objectNameLookup.end())
	{
		return iter->second.get();
	}

	return nullptr;
}