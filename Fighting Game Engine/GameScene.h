#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <boost\filesystem.hpp>

namespace FS = boost::filesystem;

class Object;
class Behaviour;
class ServiceManager;
class FilesystemManager;
class RenderingGL;
class ResourceManager;
class Debug;

class GameScene
{
protected:
	ServiceManager* _serviceManager;
	
	Debug* _debug;
	RenderingGL* _rendering;
	ResourceManager* _resource;
	FilesystemManager* _filesystem;

protected:
	std::string _name;

	bool _initialized;
	bool _loaded;

	FS::path _dataPath;

	std::vector<std::unique_ptr<Object>> _objects;
	std::unordered_map<long, Object*> _objectLookup;
	std::unordered_map<std::string, Object*> _objectNameLookup;

	std::vector<std::string> _postEffectsOrder;

	void RunFunctionOnObjectBehaviours(std::function<void(Behaviour*)> func);

	void RegisterObject(Object* obj);
	void UnregisterObject(Object* obj);
public:
	std::string name();

	bool initialized();
	bool loaded();

	const std::vector<std::string>& postEffectsOrder();

	virtual void LoadResources();
	virtual void OnLoaded();

	virtual void Init();
	virtual void Update();
	virtual void GameUpdate();
	virtual void LateGameUpdate();
	virtual void LateUpdate();
	virtual void RenderObjects();
	virtual void ApplyPostEffects();
	virtual void RenderUI();
	virtual void Cleanup();

	virtual Object* AddObject(const std::string& prefabPath);
	virtual Object* FindObject(const std::string& name);

	GameScene(const FS::path& path, ServiceManager* serviceManager);
	virtual ~GameScene();
};