#pragma once
#include <string>
#include <vector>
#include "GLIncludes.hpp"
#include <unordered_map>

enum class ShaderProgramType
{
	None = 0,
	Surface = 1,
	Compute = 2
};

class ShaderAttachment
{
	friend class GraphicsGL;

public:
	std::string code;
	GLenum type;
	ShaderAttachment(const std::string& code, GLenum type);
};

class BaseShader
{
	friend class GraphicsGL;

protected:
	std::string _name;
	GLuint _id;
	ShaderProgramType _type;

	mutable std::unordered_map<std::string, GLint> _uniformLocations;

public:
	const std::string& name() const;
	bool valid() const;

	GLint UniformLocation(const std::string& str) const;

	BaseShader(const std::string& name, GLuint id, ShaderProgramType type);
	virtual ~BaseShader();
};

