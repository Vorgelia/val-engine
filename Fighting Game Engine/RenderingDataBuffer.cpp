#include "RenderingDataBuffer.h"



void RenderingDataBuffer::SetupData(const glm::ivec2& screenSize, const glm::vec2& screenSizeInverted)
{
	_data.resize(4, 0.0f);

	_data[0] = screenSize.x;
	_data[1] = screenSize.y;
	_data[2] = screenSizeInverted.x;
	_data[3] = screenSizeInverted.y;
}

RenderingDataBuffer::RenderingDataBuffer()
{
}


RenderingDataBuffer::~RenderingDataBuffer()
{
}
