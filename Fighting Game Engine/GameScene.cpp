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

void GameScene::Deserialize(const json& j)
{
	_isLoaded = false;
	BaseObject::Deserialize(j);
	_dataJson = j;
}

void GameScene::OnInit()
{
	BaseObject::OnInit();
	_debug = _owningInstance->Debug();
	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();

	json objectJson;
	if (JSON::TryGetMember(_dataJson, "objects", objectJson))
	{
		for (const json& objData : objectJson)
		{
			AddObjectFromJson(objData);
		}
	}

	json sceneBehaviorData;
	if (JSON::TryGetMember(_dataJson, "sceneBehavior", sceneBehaviorData))
	{
		_sceneBehavior = ObjectFactory::CreateObjectFromJson<BaseSceneBehavior>(this, sceneBehaviorData);
	}

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::TimingUpdate, UpdateType::EngineUpdate, UpdateTiming);

	_isLoaded = true;
}

void GameScene::OnDestroyed()
{
	_sceneBehavior.reset();

	_objectNameLookup.clear();
	_objects.clear();

	_isLoaded = false;
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

void GameScene::RegisterReflectionFields() const
{
	BaseObject::RegisterReflectionFields();
	VE_PRIVATE_REFLECTION_VAR(Field, name);
}

void GameScene::RegisterObject(GameObject* obj)
{
	_objectNameLookup.insert(std::make_pair(obj->name(), obj));
}

void GameScene::UnregisterObject(GameObject* obj)
{
	_objectNameLookup.erase(obj->name());
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