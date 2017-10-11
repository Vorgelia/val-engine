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
public:
	static GLuint CreateShader(const std::string& code, GLenum type);
	static GLuint CreateShaderProgram(const std::vector<GLuint>& shaders);

private:
	bool _valid;

public:
	std::string name;
	std::vector<GLuint> shaders;
	std::unordered_map<std::string, GLint> uniformLocations;

	GLuint id;

	bool valid();

	GLint UniformLocation(const std::string& str);
	
	operator GLuint();
	Shader(const std::string& name, const std::vector<ShaderAttachment>& code);
	~Shader();

};


