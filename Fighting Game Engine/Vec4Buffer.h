#pragma once
#include "GraphicsBuffer.h"
class Vec4Buffer :
	public GraphicsBuffer
{
public:
	void SetupData(int size = 0, float value = 0.0f);

	Vec4Buffer();
	~Vec4Buffer();
};

