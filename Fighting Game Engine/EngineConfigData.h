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

	int frameBufferTextureAmount;
	std::string defaultFont;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, defaultResolution);
		VE_REFLECTION_VAR(Field, targetAspectRatio);
		VE_REFLECTION_VAR(Field, spriteRenderingScale);
		VE_REFLECTION_VAR(Field, uiRenderingScale);

		VE_REFLECTION_VAR(Field, useSingleBuffer);

		VE_REFLECTION_VAR(Field, frameBufferTextureAmount);
		VE_REFLECTION_VAR(Field, defaultFont);
	}
};

struct GameConfigData : public IReflectable
{
	std::string gameName;
	std::string gameManagerClassName;

	double fixedGameUpdateInterval;
	std::string defaultScene;
	int maxNetworkPlayers;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, gameName);
		VE_REFLECTION_VAR(Field, gameManagerClassName);
		VE_REFLECTION_VAR(Field, fixedGameUpdateInterval);
		VE_REFLECTION_VAR(Field, defaultScene);
		VE_REFLECTION_VAR(Field, maxNetworkPlayers);
	}
};

struct SerializationConfigData : public IReflectable
{
	std::string objectClassPropertyName;
	std::string componentListPropertyName;
	std::string prefabPathPropertyName;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, objectClassPropertyName);
		VE_REFLECTION_VAR(Field, componentListPropertyName);
		VE_REFLECTION_VAR(Field, prefabPathPropertyName);
	}
};

struct InputConfigData : public IReflectable
{
	int inputBufferLeniencyFirst;
	int inputBufferLeniencyAny;
	int inputBufferSize;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, inputBufferLeniencyAny);
		VE_REFLECTION_VAR(Field, inputBufferLeniencyFirst);
		VE_REFLECTION_VAR(Field, inputBufferSize);
	}
};

struct EngineConfigData : public IReflectable
{
	RenderingConfigData renderingConfigData;
	GameConfigData gameConfigData;
	SerializationConfigData serializationConfigData;
	InputConfigData inputConfigData;

	void RegisterReflectionFields() const override
	{
		VE_REFLECTION_VAR(Field, renderingConfigData);
		VE_REFLECTION_VAR(Field, gameConfigData);
		VE_REFLECTION_VAR(Field, serializationConfigData);
		VE_REFLECTION_VAR(Field, inputConfigData);
	}
};

