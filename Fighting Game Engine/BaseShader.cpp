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


BaseShader::BaseShader(const std::string& name, GLuint id, ShaderProgramType type)
{
	_id = id;
	_type = type;
}


BaseShader::~BaseShader()
{
}
