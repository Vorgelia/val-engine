#pragma once
#include "BaseShader.h"

class SurfaceShader : public BaseShader
{
	friend class GraphicsGL;

private:
	mutable std::unordered_map<std::string, GLint> _uniformLocations;

public:
	GLint UniformLocation(const std::string& str) const;
	
	SurfaceShader(const std::string& name, GLuint id);
	~SurfaceShader();
};