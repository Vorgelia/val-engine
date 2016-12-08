#include "Shader.h"
#include "GLStateTrack.h"
using std::string;

ShaderAttachment::ShaderAttachment(string code, GLenum type){
	this->code=code;
	this->type = type;
}

GLint Shader::UniformLocation(string str){
	//Helper function for getting a uniform location
	//Allocates it in uniformLocations when it's first required, then reads it from uniformLocations on subsequent uses.
	//Improves performance, mildly proud of coming up with this.
	if (uniformLocations.count(str) == 0){
		uniformLocations[str] = glGetUniformLocation(id,str.c_str());
	}
	return uniformLocations[str];
}
Shader::Shader(string name, std::vector<ShaderAttachment> shaders){
	this->name = name;
	std::cout << "Shader compilation [" << name.c_str() << "]:" << std::endl;
	this->shaders.reserve(shaders.size());
	for (unsigned int i = 0; i < shaders.size(); ++i)
		this->shaders.push_back(Shader::CreateShader(shaders[i].code,shaders[i].type));
	id = Shader::CreateShaderProgram(this->shaders);
	if (id == -1)
		_valid = false;
	else
		_valid = true;
	std::cout << this->name << ": " << _valid << std::endl;
}

Shader::operator GLuint(){
	return id;
}

bool Shader::valid(){
	return _valid;
}

GLuint Shader::CreateShader(std::string code, GLenum type){
	//Shader creation and debug information
	char shaderLog[512];
	GLint shaderStatus;
	GLuint shader = glCreateShader(type);
	const char* codeStr = code.c_str();
	glShaderSource(shader, 1, &codeStr,NULL);
	glCompileShader(shader);
	glGetShaderInfoLog(shader, 512, NULL, shaderLog);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus != GL_TRUE){
		std::cout << "Shader compilation failure:\n" << shaderLog << std::endl;
		return -1;
	}
	else
		std::cout << "-Success\n";
	return shader;
}

GLuint Shader::CreateShaderProgram(std::vector<GLuint> shaders){
	GLuint shaderProgram = glCreateProgram();
	for (unsigned int i = 0; i < shaders.size(); ++i){
		glAttachShader(shaderProgram,shaders[i]);
	}
	//Attach all the shaders into a shader program.
	glBindFragDataLocation(shaderProgram, 0, "OUT0");
	glBindFragDataLocation(shaderProgram, 1, "OUT1");
	glBindFragDataLocation(shaderProgram, 2, "OUT2");
	glBindFragDataLocation(shaderProgram, 3, "OUT3");
	glBindFragDataLocation(shaderProgram, 4, "OUT4");
	glBindFragDataLocation(shaderProgram, 5, "OUT5");
	glBindFragDataLocation(shaderProgram, 6, "OUT6");
	//The OUTn variables are pre-bound to point to specific targets in frameuffers.
	glBindAttribLocation(shaderProgram, 0, "vertex");
	glBindAttribLocation(shaderProgram, 1, "uv");
	glBindAttribLocation(shaderProgram, 2, "normal");
	//These make sure the locations for vertex, uv and normal are always stable.
	//These are in turn used in mesh files to specify sizes for every attribute.
	glLinkProgram(shaderProgram);
	return shaderProgram;
}

Shader::~Shader(){
	GLState::UseProgram(0);
	glDeleteProgram(id);
}
