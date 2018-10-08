#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <filesystem>
#include "BaseObject.h"

#include "ResourceManager.h"
#include "FilesystemManager.h"

namespace fs = std::filesystem;

class Behaviour;
class GameInstance;
class RenderingGL;
class ResourceManager;
class Debug;

class GameScene : public BaseObject
{
	friend class GameSceneManager;

protected:
	Debug* _debug;
	RenderingGL* _rendering;
	ResourceManager* _resource;
	FilesystemManager* _filesystem;

protected:
	FS::path _dataPath;
	std::string _name;

	bool _loaded;

	std::vector<ve::unique_object_ptr<SceneObject>> _objects;
	std::unordered_map<long, Object*> _objectLookup;
	std::unordered_map<std::string, Object*> _objectNameLookup;

	void RegisterObject(Object* obj);
	void UnregisterObject(Object* obj);

	void SortObjects();

public:
	const std::string& name() const;

	bool loaded() const;

	void LoadResources();

	virtual void OnInit() override;
	virtual void OnDestroyed() override;

	Object* LoadObject(const std::string& prefabPath);
	Object* AddObject(const json& jsonData);

	void DestroyObject(Object* object);
	void DestroyObject(int objectId);

	Object* FindObject(const std::string& name);
	Object* FindObject(int id);
	Behaviour* FindBehaviour(const std::string& name);

	GameScene();
	virtual ~GameScene();
};