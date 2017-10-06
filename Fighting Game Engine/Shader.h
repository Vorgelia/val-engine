#pragma once
#include "CommonUtilIncludes.hpp"
using std::string;

class ShaderAttachment
{
public:
	string code;
	GLenum type;
	ShaderAttachment(const string& code, GLenum type);
};

class Shader
{
private:
	bool _valid;
public:
	string name;
	std::vector<GLuint> shaders;
	std::map<string, GLint> uniformLocations;

	GLuint id;

	bool valid();
	GLint UniformLocation(const string& str);
	Shader(const string& name, const std::vector<ShaderAttachment>& code);
	operator GLuint();
	static GLuint CreateShader(const std::string& code, GLenum type);
	static GLuint CreateShaderProgram(const std::vector<GLuint>& shaders);
	~Shader();

};


