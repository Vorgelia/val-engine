#pragma once
#include <vector>

namespace ValEngine
{
	template<typename ContainerT>
	void erase_swap(ContainerT& container, int index)
	{
		if(container.size() <= 1)
		{
			container.clear();
			return;
		}

		std::swap(container.begin() + index, container.back());
		container.pop_back();
	}
}
