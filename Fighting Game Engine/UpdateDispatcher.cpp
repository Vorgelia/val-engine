#include "UpdateDispatcher.h"
#include "ValEngine.h"
#include "ContainerUtils.h"
#include "GameInstance.h"
#include "GameSceneManager.h"

VE_OBJECT_DEFINITION(UpdateDispatcher);

void UpdateDispatcher::Init()
{
	_fixedGameUpdateInterval = _owningInstance->configData().gameConfigData.fixedGameUpdateInterval;

	ResetFixedGameUpdateTime();

	auto& sceneLoadedDelegate = _owningInstance->GameSceneManager()->SceneLoaded;
	sceneLoadedDelegate += VE_DELEGATE_FUNC(GameSceneManager::GameSceneEventHandler, HandleSceneLoaded);
}

void UpdateDispatcher::SortFunctions()
{
	for(size_t i = 0; i < _boundFunctions.size();)
	{
		if(!_boundFunctions[i].IsValid())
		{
			ve::erase_swap(_boundFunctions, i);
			continue;
		}
		++i;
	}

	std::sort(_boundFunctions.begin(), _boundFunctions.end(), [](const TrackedUpdateFunction& a, const TrackedUpdateFunction& b)
	{
		return a.timing.updateGroup < b.timing.updateGroup;
	});
}

void UpdateDispatcher::ResetFixedGameUpdateTime()
{
	_lastFixedGameUpdateTime = _owningInstance->timeTracker().time() + _lastFixedGameUpdateTime;
}

void UpdateDispatcher::HandleSceneLoaded(const GameScene* scene)
{
	_justLoadedLevel = true;
}

void UpdateDispatcher::RunUpdateFunctionsOfType(UpdateType type)
{
	for(auto& func : _boundFunctions)
	{
		if((func.timing.updateType | type) != UpdateType::None)
		{
			if(!func.Invoke())
			{
				_shouldSortFunctions = true;
			}
		}
	}
}

void UpdateDispatcher::BindFunction(BaseObject* object, UpdateFunctionTiming timing, TrackedUpdateFunction::func_t function)
{
	_boundFunctions.emplace_back(object, std::move(timing), std::move(function));
	_shouldSortFunctions = true;
}

void UpdateDispatcher::UnbindFunction(BaseObject* object)
{
	for(size_t i = 0; i < _boundFunctions.size();)
	{
		if(_boundFunctions[i].object.get() == object)
		{
			ve::erase_swap(_boundFunctions, i);
			continue;
		}
		++i;
	}
}

void UpdateDispatcher::DispatchUpdates()
{
	if(!_initialized)
	{
		Init();
	}

	if(_shouldSortFunctions)
	{
		SortFunctions();
		_shouldSortFunctions = false;
	}

	if(_justLoadedLevel)
	{
		ResetFixedGameUpdateTime();
		_justLoadedLevel = false;
		return;
	}

	const TimeTracker& time = _owningInstance->timeTracker();

	int gameUpdateAmount = 0;
	if(_fixedGameUpdateInterval > 0 && glm::nearlyZero(_fixedGameUpdateInterval))
	{
		gameUpdateAmount = int(glm::floor((_lastFixedGameUpdateTime - time.time()) / _fixedGameUpdateInterval));
	}

	int updateAmount = 0;
	while(updateAmount < glm::max(1, gameUpdateAmount))
	{
		UpdateType updateFunctionType = UpdateType::None;

		if(updateAmount == 0)
		{
			updateFunctionType |= UpdateType::EngineUpdate;
		}

		if(updateAmount < gameUpdateAmount)
		{
			updateFunctionType |= UpdateType::AnyFixedGameUpdate;
			if(updateAmount == 0)
			{
				updateFunctionType |= UpdateType::FirstFixedGameUpdate;
			}
			if(updateAmount == gameUpdateAmount - 1)
			{
				updateFunctionType |= UpdateType::LastFixedGameUpdate;
			}
		}

		RunUpdateFunctionsOfType(updateFunctionType);

		++updateAmount;
	}
}

UpdateDispatcher::UpdateDispatcher()
	: _shouldSortFunctions(false)
	, _fixedGameUpdateInterval(1.0 / 60.0)
	, _lastFixedGameUpdateTime(-1.0f)
	, _initialized(false)
	, _justLoadedLevel(false)
{

}
