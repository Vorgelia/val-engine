#pragma once
#include <vector>
#include "GLIncludes.hpp"

enum class GraphicsBufferType
{
	Uniform,
	ShaderStorage
};

class GraphicsBuffer
{
	friend class GraphicsGL;

private:
	GLuint _id;
	mutable GLuint _dataSize;

	GraphicsBufferType _type;

protected:
	std::vector<float> _data;

public:
	virtual const std::vector<float>& data() const;

	GraphicsBuffer();
	~GraphicsBuffer();
};

