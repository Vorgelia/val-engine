#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "GLIncludes.hpp"

enum class ShaderProgramType
{
	None = 0,
	Surface = 1,
	Compute = 2
};

class ShaderAttachment
{
public:
	std::string code;
	GLenum type;
	ShaderAttachment(const std::string& code, GLenum type);
};

class Shader
{
	friend class GraphicsGL;

private:
	std::string _name;
	GLuint _id;
	ShaderProgramType _type;
	bool _valid;

	mutable std::unordered_map<std::string, GLint> _uniformLocations;

public:
	const std::string& name() const;
	bool valid() const;

	GLint UniformLocation(const std::string& str) const;
	
	Shader(const std::string& name, GLuint id, ShaderProgramType type);
	~Shader();
};