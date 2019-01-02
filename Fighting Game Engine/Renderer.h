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
	virtual std::vector<RenderingCommand> GetRenderingCommands() const = 0;

	BaseRenderer() = default;
	~BaseRenderer() = default;
};

class Renderer : public BaseRenderer
{
	VE_OBJECT_DECLARATION(Renderer)

protected:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

public:
	virtual json Serialize() const;
	virtual void Deserialize(const json& j);

	std::vector<RenderingCommand> GetRenderingCommands() const override;

	Renderer() = default;
	~Renderer() = default;
};
