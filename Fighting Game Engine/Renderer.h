#pragma once
#include "ObjectComponent.h"
#include <string>
#include "RenderingCommand.h"

class ResourceManager;
class RenderingGL;

class BaseRenderer : public ObjectComponent
{
	VE_DECLARE_OBJECT(BaseRenderer)

public:
	virtual std::vector<RenderingCommand> GetRenderingCommands() const = 0;

	BaseRenderer();
	~BaseRenderer() = default;
};

class Renderer : public BaseRenderer
{
	VE_DECLARE_OBJECT(Renderer)

protected:
	Mesh* _mesh;
	Material* _material;

public:
	virtual json Serialize() const;
	virtual void Deserialize(const json& j);

	std::vector<RenderingCommand> GetRenderingCommands() const override;

	Renderer() = default;
	~Renderer() = default;
};
