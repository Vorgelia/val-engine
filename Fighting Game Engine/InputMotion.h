#pragma once
#include <vector>
#include "JSON.h"

class InputMotionComponent;

class InputMotion
{
	friend class InputDevice;
	std::vector<InputMotionComponent> _components;

public:
	InputMotion(const json& j);
	InputMotion(std::vector<InputMotionComponent> components);
	~InputMotion();
};

