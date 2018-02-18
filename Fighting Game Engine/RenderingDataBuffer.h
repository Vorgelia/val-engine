#pragma once
#include "GraphicsBuffer.h"
#include "MathIncludes.hpp"

class RenderingDataBuffer :
	public GraphicsBuffer
{
public:
	void SetupData(const glm::ivec2& screenSize, const glm::vec2& screenSizeInverted);

	RenderingDataBuffer();
	~RenderingDataBuffer();
};

