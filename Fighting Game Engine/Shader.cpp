#include "Shader.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

ShaderAttachment::ShaderAttachment(const std::string& code, GLenum type)
{
	this->code = code;
	this->type = type;
}

GLint Shader::UniformLocation(const std::string& str) const
{
	auto& iter = _uniformLocations.find(str);
	if(iter != _uniformLocations.end())
	{
		return iter->second;
	}

	return _uniformLocations.insert(
		std::make_pair(str
			, glGetUniformLocation(_id, str.c_str())
		)).first->second;
}

const std::string& Shader::name() const
{
	return _name;
}

bool Shader::valid() const
{
	return _valid;
}

Shader::Shader(const std::string & name, GLuint id, ShaderProgramType type)
{
	_id = id;
	_type = type;
	_valid = id > 0;
}

Shader::~Shader()
{

}
