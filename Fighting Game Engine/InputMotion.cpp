#include "InputMotion.h"
#include "InputMotionComponent.h"

InputMotion::InputMotion(const json & j)
{
	_components.reserve(j.size());
	for(auto& iter : j)
	{
		_components.push_back(InputMotionComponent(iter));
	}
}

InputMotion::InputMotion(const std::vector<InputMotionComponent>& components) :
	_components(components)
{
}

InputMotion::~InputMotion()
{
}
