#pragma once
#include "ValEngine.h"
#include "IReflectable.h"

struct RenderingConfigData : public IReflectable
{
	ve::ivec2 defaultResolution;
	ve::ivec2 targetAspectRatio;
	ve::ivec2 spriteRenderingScale;
	ve::ivec2 uiRenderingScale;

	bool useSingleBuffer;

	int frameBuferTextureAmount;
	std::string defaultFont;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, defaultResolution);
		VE_REFLECTION_VAR(Field, targetAspectRatio);
		VE_REFLECTION_VAR(Field, spriteRenderingScale);
		VE_REFLECTION_VAR(Field, uiRenderingScale);

		VE_REFLECTION_VAR(Field, useSingleBuffer);

		VE_REFLECTION_VAR(Field, frameBuferTextureAmount);
		VE_REFLECTION_VAR(Field, defaultFont);
	}
};

struct GameConfigData : public IReflectable
{
	std::string gameName;
	double fixedGameUpdateInterval;
	std::string defaultScene;
	int maxNetworkPlayers;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, gameName);
		VE_REFLECTION_VAR(Field, fixedGameUpdateInterval);
		VE_REFLECTION_VAR(Field, defaultScene);
		VE_REFLECTION_VAR(Field, maxNetworkPlayers);
	}
};

struct SerializationConfigData : public IReflectable
{
	std::string objectClassPropertyName;
	std::string objectListPropertyName;
	std::string componentListPropertyName;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, objectClassPropertyName);
		VE_REFLECTION_VAR(Field, objectListPropertyName);
		VE_REFLECTION_VAR(Field, componentListPropertyName);
	}
};

struct EngineConfigData : public IReflectable
{
	RenderingConfigData renderingConfigData;
	GameConfigData gameConfigData;
	SerializationConfigData serializationConfigData;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, renderingConfigData);
		VE_REFLECTION_VAR(Field, gameConfigData);
		VE_REFLECTION_VAR(Field, serializationConfigData);
	}
};

