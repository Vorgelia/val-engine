#pragma once
#include "IReflectable.h"

struct RenderingConfigData : public IReflectable
{
	bool useSingleBuffer;
	ve::ivec2 defaultResolution;

	void RegisterReflectionFields() const override
	{
		AddReflectionField(VE_REFLECTION_ARG(defaultResolution));
		AddReflectionField(VE_REFLECTION_ARG(useSingleBuffer));
	}
};

struct GameConfigData : public IReflectable
{
	double fixedGameUpdateInterval;
	std::string defaultScene;

	void RegisterReflectionFields() const override
	{
		AddReflectionField(VE_REFLECTION_ARG(fixedGameUpdateInterval));
		AddReflectionField(VE_REFLECTION_ARG(defaultScene));
	}
};


struct EngineConfigData : public IReflectable
{
	RenderingConfigData renderingConfigData;
	GameConfigData gameConfigData;
	
	void RegisterReflectionFields() const override
	{
		AddReflectionField(VE_REFLECTION_ARG(renderingConfigData));
		AddReflectionField(VE_REFLECTION_ARG(gameConfigData));
	}
};

