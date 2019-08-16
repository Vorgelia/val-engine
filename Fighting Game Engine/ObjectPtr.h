#pragma once
#include <memory>
class BaseObject;

namespace ValEngine
{
	struct ObjectDeleter
	{
		void operator()(BaseObject* object) const;
	};

	template <typename ObjectT>
	using unique_object_ptr = std::unique_ptr<ObjectT, ObjectDeleter>;
}
