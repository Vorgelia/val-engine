#include "SurfaceShader.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

GLint SurfaceShader::UniformLocation(const std::string& str) const
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

SurfaceShader::SurfaceShader(const std::string & name, GLuint id)
	: BaseShader(name, id, ShaderProgramType::Surface)
{

}

SurfaceShader::~SurfaceShader()
= default;
