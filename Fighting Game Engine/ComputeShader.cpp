#include "ComputeShader.h"



const glm::ivec3 & ComputeShader::ComputeGroupSize()
{
	return _computeGroupSize;
}

ComputeShader::ComputeShader(const std::string& name, GLuint id)
	:BaseShader(name, id, ShaderProgramType::Compute)
{
}


ComputeShader::~ComputeShader()
{
}
