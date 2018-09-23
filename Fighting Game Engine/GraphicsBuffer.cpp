#include "GraphicsBuffer.h"

const std::vector<float>& GraphicsBuffer::data() const
{
	return _data;
}

GraphicsBuffer::GraphicsBuffer()
{
	_id = 0;
}

GraphicsBuffer::~GraphicsBuffer()
= default;
