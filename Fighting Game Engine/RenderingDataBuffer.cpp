#include "RenderingDataBuffer.h"



void RenderingDataBuffer::SetupData(const glm::ivec2& screenSize, const glm::vec2& screenSizeInverted)
{
	_data.resize(4, 0.0f);

	_data[0] = float(screenSize.x);
	_data[1] = float(screenSize.y);
	_data[2] = screenSizeInverted.x;
	_data[3] = screenSizeInverted.y;
}

RenderingDataBuffer::RenderingDataBuffer()
= default;


RenderingDataBuffer::~RenderingDataBuffer()
= default;
