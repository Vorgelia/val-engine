#pragma once
#include "BaseShader.h"
#include "MathIncludes.hpp"

class ComputeShader :
	public BaseShader
{
	friend class GraphicsGL;

private:
	glm::ivec3 _computeGroupSize;

public:
	const glm::ivec3& ComputeGroupSize() const;

	ComputeShader(const std::string& name, GLuint id);
	~ComputeShader();
};

