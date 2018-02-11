#pragma once

enum class ImageBindingPoints
{
	MainBufferAttachment0 = 0,
	AuxBuffer2Attachment0 = 3,
	FirstUnreservedImagePoint = 6
};

enum class UniformBufferBindingPoints
{
	TimeDataBuffer = 0,
	RenderingDataBuffer = 1,
	FirstUnreservedBufferPoint = 2,
};

enum class ShaderStorageBufferBindingPoints
{
	CommonVec4Buffer1024 = 1,
	FirstUnreservedBufferPoint = 2,
};
