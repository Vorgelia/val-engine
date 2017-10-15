#pragma once
#include <map>
#include <vector>
#include <string>
#include <boost\filesystem.hpp>

#include "Object.h"

namespace FS = boost::filesystem;

class Object;
class GameState
{
protected:
	bool _initialized;
	bool _loaded;

	FS::path _dataPath;

	std::vector<std::unique_ptr<Object>> _objects;
	std::unordered_map<long, Object*> _objectLookup;
	std::unordered_map<std::string, Object*> _objectNameLookup;

	std::vector<std::string> _postEffectsOrder;
public:

	bool initialized();
	bool loaded();

	const std::vector<std::string>& postEffectsOrder();

	virtual void LoadResources();
	virtual void OnLoaded();

	virtual void Init();
	virtual void Update();
	virtual void GameUpdate();
	virtual void RenderObjects();
	virtual void FrameEnd();
	virtual void GUI();

	virtual void Cleanup();
	virtual std::string GameState::Serialize();
	virtual void GameState::Deserialize(const std::string& data);
	virtual Object* FindObject(const std::string& name);

	GameState(const FS::path& path);
	virtual ~GameState();
};

