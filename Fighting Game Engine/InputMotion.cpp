#include "InputMotion.h"
#include <utility>
#include "InputMotionComponent.h"

InputMotion::InputMotion(const json & j)
{
	_components.reserve(j.size());
	for(auto& iter : j)
	{
		_components.emplace_back(iter);
	}
}

InputMotion::InputMotion(std::vector<InputMotionComponent> components) :
	_components(std::move(components))
{
}

InputMotion::~InputMotion()
= default;
