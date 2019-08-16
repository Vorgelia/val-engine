#pragma once
#include "ObjectComponent.h"
#include <string>
#include "RenderingCommand.h"

class ResourceManager;
class RenderingGL;
class Material;

class BaseRenderer : public ObjectComponent
{
public:
	virtual std::vector<RenderingCommand> GetRenderingCommands(const BaseCamera* camera) const = 0;

	BaseRenderer() = default;
	~BaseRenderer() = default;
};

class Renderer : public BaseRenderer
{
	VE_OBJECT_DECLARATION(Renderer, BaseRenderer)

protected:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

public:
	std::vector<RenderingCommand> GetRenderingCommands(const BaseCamera* camera) const override;
	void OnDeserialized(BaseSerializationProxy& proxy) override;

	Renderer() = default;
	~Renderer() = default;
};

class ParallaxRenderer : public Renderer
{
	VE_OBJECT_DECLARATION(ParallaxRenderer, Renderer)

protected:
	float _parallaxScale = 1.0f;

public:
	virtual void RegisterReflectionFields() const override;

	std::vector<RenderingCommand> GetRenderingCommands(const BaseCamera* camera) const override;

	ParallaxRenderer() = default;
	~ParallaxRenderer() = default;
};