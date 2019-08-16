#pragma once
#include "TrackedUpdateFunction.h"
#include "Time.h"
#include "BaseObject.h"

class GameInstance;
class GameScene;

class UpdateDispatcher : public BaseObject
{
	VE_OBJECT_DECLARATION(UpdateDispatcher, BaseObject)

protected:
	std::vector<TrackedUpdateFunction> _boundFunctions;
	std::vector<TrackedUpdateFunction> _pendingBoundFunctions;

	bool _shouldSortFunctions;
	bool _isDispatchingUpdates;

	double _fixedGameUpdateInterval;
	double _lastFixedGameUpdateTime;

	int _gameUpdatesPerFrame;

	bool _justLoadedLevel;

	bool _initialized;

	void Init();
	void SortFunctions();

	void ResetFixedGameUpdateTime();

	void HandleSceneLoaded(const GameScene* scene);

	void RunUpdateFunctionsOfType(UpdateType type);

public:
	int gameUpdatesPerFrame() const { return _gameUpdatesPerFrame; }

	void BindFunction(BaseObject* object, UpdateFunctionTiming timing, TrackedUpdateFunction::func_t function);
	void UnbindFunction(BaseObject* object);
	void DispatchUpdates();

	UpdateDispatcher();
};
