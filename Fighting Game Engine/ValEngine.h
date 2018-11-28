#pragma once
#include <cstdint>
#include "Math.h"
#include "BaseObject.h"
#include "ObjectReferenceManager.h"

//TODO: Add/move more stuff here.

namespace ValEngine
{
	bool IsValid(const BaseObject* object) { return ObjectReferenceManager::Get().IsObjectValid(object); }
}

namespace ve = ValEngine;