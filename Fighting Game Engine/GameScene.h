#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <filesystem>
#include "BaseObject.h"

#include "GameInstance.h"

namespace fs = std::filesystem;

class GameInstance;
class RenderingGL;
class ResourceManager;
class FilesystemManager;
class Debug;
class BaseSceneBehavior;

class GameScene : public BaseObject
{
	VE_OBJECT_DECLARATION(GameScene);

	friend class GameSceneManager;
	friend class ObjectInitializer;

protected:
	Debug* _debug;
	RenderingGL* _rendering;
	ResourceManager* _resource;

protected:
	fs::path _dataPath;
	std::string _name;

	bool _loaded;

	ve::unique_object_ptr<BaseSceneBehavior> _sceneBehavior;

	std::vector<ve::unique_object_ptr<GameObject>> _objects;
	std::unordered_map<std::string, ObjectReference<GameObject>> _objectNameLookup;

	TimeTracker _timeTracker;

	void RegisterObject(GameObject* obj);
	void UnregisterObject(GameObject* obj);

	void LoadResources();

public:
	const std::string& name() const;
	bool loaded() const;
	BaseSceneBehavior* sceneBehavior() const { return _sceneBehavior.get(); }

	const TimeTracker& GetTime() const { return _timeTracker; }

	virtual void OnInit() override;
	virtual void OnDestroyed() override;

	void UpdateTiming();

	template<typename ObjectT = GameObject>
	ObjectReference<ObjectT> AddObject(const json& jsonData = json());
	ObjectReference<GameObject> AddObjectFromJson(const json& jsonData = json());
	ObjectReference<GameObject> LoadObject(const std::string& prefabPath);

	GameObject* FindObject(const std::string& name);
	template<typename ObjectT>
	ObjectReference<ObjectT> FindObjectOfType();
	template<typename ObjectT>
	std::vector<ObjectReference<ObjectT>> FindObjectsOfType();

	void DestroyObject(GameObject* object);

	GameScene() = default;
	virtual ~GameScene() = default;
};

template <typename ObjectT>
ObjectReference<ObjectT> GameScene::AddObject(const json& jsonData)
{
	static_assert(std::is_base_of_v<GameObject, ObjectT>, "Objects added to a scene need to derive from GameObject.");
	_objects.push_back(ObjectFactory::CreateObject<ObjectT>(this, jsonData));

	ObjectT* result = _objects.back().get();
	RegisterObject(result);

	return ObjectReference<ObjectT>(result);
}

template <typename ObjectT>
ObjectReference<ObjectT> GameScene::FindObjectOfType()
{
	for(auto& iter : _objects)
	{
		ObjectT* obj = nullptr;
		if constexpr(std::is_base_of_v<ObjectComponent, ObjectT>)
		{
			obj = iter.get()->GetComponentOfType<ObjectT>().get();
		}
		else
		{
			obj = dynamic_cast<ObjectT*>(iter.get());
		}

		if(obj != nullptr)
		{
			return ObjectReference<ObjectT>(obj);
		}
	}

	return ObjectReference<ObjectT>(nullptr);
}

template <typename ObjectT>
std::vector<ObjectReference<ObjectT>> GameScene::FindObjectsOfType()
{
	std::vector<ObjectReference<ObjectT>> objects;

	for(auto& iter : _objects)
	{
		if(iter.get() == nullptr)
		{
			__debugbreak();
			continue;
		}

		if constexpr(std::is_base_of_v<ObjectComponent, ObjectT>)
		{
			std::vector<ObjectReference<ObjectT>> components = iter.get()->GetComponentsOfType<ObjectT>();
			if(components.size() > 0)
			{
				objects.reserve(components.size());
				std::move(components.begin(), components.end(), objects.end());
			}
		}
		else
		{
			ObjectT* obj = dynamic_cast<ObjectT>(iter.get());
			if(obj != nullptr)
			{
				objects.push_back(ObjectReference<ObjectT>(obj));
			}
		}
	}

	return objects;
}
