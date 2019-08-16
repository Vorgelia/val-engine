#include "GameScene.h"
#include "GameSceneManager.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "RenderingGL.h"
#include "BaseSceneBehavior.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "DebugLog.h"
#include "SerializationProxy.h"

VE_OBJECT_DEFINITION(GameScene);

void GameScene::RegisterReflectionFields() const
{
	BaseObject::RegisterReflectionFields();
	VE_PRIVATE_REFLECTION_VAR(Field, name);
	VE_PRIVATE_REFLECTION_VAR(ObjectArray, objects, this);
	VE_PRIVATE_REFLECTION_VAR(Object, sceneBehavior, this);
}

void GameScene::OnDeserialized(BaseSerializationProxy& proxy)
{
	_isLoaded = false;
	for (auto& object : _objects)
		RegisterObject(object.get());
}

void GameScene::OnInit()
{
	BaseObject::OnInit();
	_debug = _owningInstance->Debug();
	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();

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

void GameScene::RegisterObject(GameObject* obj)
{
	_objectNameLookup.insert(std::make_pair(obj->name(), obj));
}

void GameScene::UnregisterObject(GameObject* obj)
{
	_objectNameLookup.erase(obj->name());
}

ObjectReference<GameObject> GameScene::AddObjectFromJson(json& jsonData)
{
	JsonSerializationProxy proxy(jsonData);

	_objects.push_back(ObjectFactory::CreateObjectFromData<GameObject>(this, proxy));
	GameObject* result = _objects.back().get();
	RegisterObject(result);

	return ObjectReference<GameObject>(result);
}

ObjectReference<GameObject> GameScene::LoadObject(const std::string& prefabPath)
{
	json* data = _resource->GetJsonData(prefabPath);
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