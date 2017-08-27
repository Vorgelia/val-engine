#pragma once
#include "CommonUtilIncludes.hpp"
class Object;
class GameState
{
protected:
	bool _initialized;
	bool _loaded;
public:

	std::map<int, Object*> objects;
	FS::path dataPath;

	std::vector<std::string> postEffectsOrder;

	bool initialized();
	bool loaded();
	virtual void OnLoaded();
	virtual void Init();
	virtual void LoadResources();
	virtual void Update();
	virtual void GameUpdate();
	virtual void RenderObjects();
	virtual void FrameEnd();
	virtual void GUI();
	virtual void Cleanup();
	virtual std::string GameState::Serialize();
	virtual void GameState::Deserialize(std::string data);
	virtual const Object* FindObject(std::string name);
	GameState(FS::path path);
	virtual ~GameState();
};

