#include "BaseShader.h"

ShaderAttachment::ShaderAttachment(const std::string& code, GLenum type)
{
	this->code = code;
	this->type = type;
}

const std::string& BaseShader::name() const
{
	return _name;
}

bool BaseShader::valid() const
{
	return _id > 0;
}

GLint BaseShader::UniformLocation(const std::string& str) const
{
	auto iter = _uniformLocations.find(str);
	if(iter != _uniformLocations.end())
	{
		return iter->second;
	}

	return _uniformLocations.insert(
		std::make_pair(str
			, glGetUniformLocation(_id, str.c_str())
		)).first->second;
}

BaseShader::BaseShader(const std::string& name, GLuint id, ShaderProgramType type)
{
	_id = id;
	_type = type;
}


BaseShader::~BaseShader()
= default;
