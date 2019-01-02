#include "GameScene.h"
#include "GameSceneManager.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "RenderingGL.h"
#include "BaseSceneBehavior.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "DebugLog.h"

VE_OBJECT_DEFINITION(GameScene);

void GameScene::LoadResources()
{
	_loaded = false;
	if(_dataPath.empty())
	{
		_loaded = true;
		return;
	}

	json* sceneData = _resource->GetJsonData(_dataPath.string());
	if(sceneData == nullptr)
	{
		_loaded = true;
		return;
	}

	json objectJson;
	if(JSON::TryGetMember(*sceneData, "objects", objectJson))
	{
		for(const json& objData : objectJson)
		{
			AddObjectFromJson(objData);
		}
	}

	json sceneBehaviorData;
	if(JSON::TryGetMember(*sceneData, "sceneBehavior", sceneBehaviorData))
	{
		_sceneBehavior = ObjectFactory::CreateObjectFromJson<BaseSceneBehavior>(this, sceneBehaviorData);
	}

	_loaded = true;
}

void GameScene::OnInit()
{
	BaseObject::OnInit();
	_debug = _owningInstance->Debug();
	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();

	LoadResources();

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::TimingUpdate, UpdateType::EngineUpdate, UpdateTiming);
}

void GameScene::OnDestroyed()
{
	_objects.clear();
	_objectNameLookup.clear();

	_loaded = false;
}

void GameScene::UpdateTiming()
{
	if(!_updatedTiming)
	{
		_timeTracker.Reset(_owningInstance->timeTracker().time());
		_updatedTiming = true;
	}
	_timeTracker.Update(_owningInstance->timeTracker());
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

ObjectReference<GameObject> GameScene::AddObjectFromJson(const json& jsonData)
{
	std::string prefabPath;
	json objectJson = jsonData;
	if(JSON::TryGetMember(jsonData, _owningInstance->configData().serializationConfigData.prefabPathPropertyName, prefabPath))
	{
		const json* data = _resource->GetJsonData(prefabPath);
		if(data == nullptr)
		{
			return ObjectReference<GameObject>();
		}

		objectJson = *data;
		objectJson.merge_patch(jsonData);
	}

	_objects.push_back(ObjectFactory::CreateObjectFromJson<GameObject>(this, objectJson));
	GameObject* result = _objects.back().get();
	RegisterObject(result);

	return ObjectReference<GameObject>(result);
}

ObjectReference<GameObject> GameScene::LoadObject(const std::string& prefabPath)
{
	const json* data = _resource->GetJsonData(prefabPath);
	if(data == nullptr)
	{
		return ObjectReference<GameObject>();
	}

	return AddObjectFromJson(*data);
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