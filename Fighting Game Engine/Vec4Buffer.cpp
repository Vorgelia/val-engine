#include "Vec4Buffer.h"



void Vec4Buffer::SetupData(int size, float value)
{
	_data.resize(size);
	for(float& element : _data)
	{
		element = value;
	}
}

Vec4Buffer::Vec4Buffer()
= default;


Vec4Buffer::~Vec4Buffer()
= default;
