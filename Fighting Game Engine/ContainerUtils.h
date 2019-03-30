#pragma once
#include <vector>
#include "ValEngine.h"

namespace ValEngine
{
	template<typename ContainedT>
	void erase_swap(std::vector<ContainedT>& container, size_t index)
	{
		if(container.size() <= 1)
		{
			container.clear();
		}
		else if(index >= container.size())
		{
			return;
		}
		else if(index == container.size() - 1)
		{
			container.pop_back();
		}
		else
		{
			std::swap(container[index], container[container.size() - 1]);
			container.pop_back();
		}

	}
}
