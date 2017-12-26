#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "GLIncludes.hpp"

class ShaderAttachment
{
public:
	std::string code;
	GLenum type;
	ShaderAttachment(const std::string& code, GLenum type);
};

class Shader
{
private:
	bool _valid;

	std::unordered_map<std::string, GLint> _uniformLocations;
public:
	std::string name;

	GLuint id;

	bool valid();

	GLint UniformLocation(const std::string& str);
	
	Shader(const std::string& name, GLuint id);
	~Shader();

};


