#pragma once

enum class ImageBindingPoints
{
	MainBufferAttachment0 = 0,
	AuxBuffer2Attachment0 = 3,
	FirstUnreservedImagePoint = 6
};

enum class UniformBlockBindingPoints
{
	TimeDataBuffer = 0,
	RenderingDataBuffer = 1,
	FirstUnreservedBlockPoint = 2,
};

enum class ShaderStorageBlockBindingPoints
{
	CommonVec4Buffer = 0,
	FirstUnreservedBlockPoint = 1,
};