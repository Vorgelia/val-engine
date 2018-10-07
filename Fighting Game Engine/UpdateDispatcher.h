#pragma once
#include "TrackedUpdateFunction.h"
#include "Time.h"

class GameInstance;

struct UpdateDispatcher
{
protected:
	GameInstance* _gameInstance;

	std::vector<TrackedUpdateFunction> _boundFunctions;
	bool _shouldSortFunctions;

	double _fixedGameUpdateInterval;
	double _lastFixedGameUpdateTime;

	bool _initialized;

	void Init();
	void SortFunctions();

	void RunUpdateFunctionsOfType(UpdateType type);

public:
	void BindFunction(BaseObject* object, UpdateFunctionTiming timing, TrackedUpdateFunction::func_t function);
	void UnbindFunction(BaseObject* object);
	void DispatchUpdates();

	explicit UpdateDispatcher(GameInstance* gameInstance);
};
