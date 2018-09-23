#pragma once
#include "BaseShader.h"
#include <unordered_map>

class SurfaceShader : public BaseShader
{
	friend class GraphicsGL;

public:
	SurfaceShader(const std::string& name, GLuint id);
	~SurfaceShader();
};
