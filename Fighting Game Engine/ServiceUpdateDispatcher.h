#pragma once
#include "ValEngine.h"
#include "EnumUtils.h"
#include <functional>

class BaseService;

enum class UpdateGroup
{
	FrameStart = 100,
	FrameUpdate = 200,
	Rendering = 300,
	PostProcess = 400,
	UI = 500,
	FrameEnd = 600
};

enum class UpdateType : ve::uint8_t
{
	VE_BITMASK_VALUE_NONE,
	EngineUpdate = 0b01,
	FixedGameUpdate = 0b10,
	VE_BITMASK_VALUE_ANY
};
VE_DECLARE_BITMASK_ENUM(UpdateType)

struct ServiceUpdateFunction
{
public:
	typedef std::function<void()> func_t;

	UpdateGroup updateGroup;
	UpdateType updateType;
	func_t function;

	ServiceUpdateFunction(UpdateGroup inUpdateGroup, UpdateType inUpdateType, func_t inFunction)
		: updateGroup(inUpdateGroup)
		, updateType(inUpdateType)
		, function(std::move(inFunction))
	{ }
};

struct ServiceUpdateDispatcher
{
protected:
	class GameInstance* _gameInstance;
	std::vector<ServiceUpdateFunction> _boundFunctions;

public:

	void BindFunction(ServiceUpdateFunction function);

	void DispatchUpdates();

	explicit ServiceUpdateDispatcher(GameInstance* gameInstance);
};
