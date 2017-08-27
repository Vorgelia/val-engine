#pragma once
#include "CommonUtilIncludes.hpp"
using std::string;

class ShaderAttachment
{
public:
	string code;
	GLenum type;
	ShaderAttachment(string code, GLenum type);
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
	GLint UniformLocation(string str);
	Shader(string n, std::vector<ShaderAttachment> code);
	operator GLuint();
	static GLuint CreateShader(std::string code, GLenum type);
	static GLuint CreateShaderProgram(std::vector<GLuint> shaders);
	~Shader();

};


