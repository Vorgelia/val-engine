#pragma once
#include <functional>
#include "EnumUtils.h"
#include "ValEngine.h"
#include "ObjectReferenceManager.h"

class BaseObject;

enum class UpdateGroup
{
	TimingUpdate = 0,
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
	EngineUpdate			= 0b0001,
	FirstFixedGameUpdate	= 0b0010,
	AnyFixedGameUpdate		= 0b0100,
	LastFixedGameUpdate		= 0b1000,
	VE_BITMASK_VALUE_ANY
};

VE_DECLARE_BITMASK_ENUM(UpdateType);

struct UpdateFunctionTiming
{
public:
	UpdateGroup updateGroup;
	UpdateType updateType;

	UpdateFunctionTiming(UpdateGroup inUpdateGroup, UpdateType inUpdateType)
		: updateGroup(inUpdateGroup)
		, updateType(inUpdateType)
	{ }
};

struct TrackedUpdateFunction
{
public:
	typedef std::function<void()> func_t;

	ObjectReference<> object;
	UpdateFunctionTiming timing;
	func_t function;

	bool IsValid() const { return object.get() != nullptr; }
	bool Invoke()
	{
		if(IsValid())
		{
			function();
			return true;
		}

		return false;
	}

	TrackedUpdateFunction(BaseObject* inObject, UpdateFunctionTiming inTiming, func_t inFunction)
		: object(inObject)
		, timing(std::move(inTiming))
		, function(std::move(inFunction))
	{
	}
};