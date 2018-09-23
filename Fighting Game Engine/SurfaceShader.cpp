#include "SurfaceShader.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

SurfaceShader::SurfaceShader(const std::string & name, GLuint id)
	: BaseShader(name, id, ShaderProgramType::Surface)
{

}

SurfaceShader::~SurfaceShader() = default;
