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

class GameScene
{
protected:
	bool _initialized;
	bool _loaded;

	FS::path _dataPath;

	std::vector<std::unique_ptr<Object>> _objects;
	std::unordered_map<long, Object*> _objectLookup;
	std::unordered_map<std::string, Object*> _objectNameLookup;

	std::vector<std::string> _postEffectsOrder;

	void RunFunctionOnObjectBehaviours(std::function<void(Behaviour*)> func);
public:

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
	virtual Object* FindObject(const std::string& name);

	GameScene(const FS::path& path);
	virtual ~GameScene();
};