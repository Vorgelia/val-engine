#include "Shader.h"
#include "GLStateTrack.h"
#include "DebugLog.h"

ShaderAttachment::ShaderAttachment(const std::string& code, GLenum type)
{
	this->code = code;
	this->type = type;
}

GLint Shader::UniformLocation(const std::string& str)
{
	auto& iter = _uniformLocations.find(str);
	if(iter != _uniformLocations.end())
	{
		return iter->second;
	}

	return _uniformLocations.insert(
		std::make_pair(str
			, glGetUniformLocation(id, str.c_str())
		)).first->second;
}


bool Shader::valid()
{
	return _valid;
}

Shader::Shader(const std::string & name, GLuint id)
{
	this->id = id;
	_valid = id > 0;
}

Shader::~Shader()
{

}
