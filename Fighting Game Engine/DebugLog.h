#pragma once
#include <string>
#include <vector>
#include <memory>
#include "LogItem.h"

#define VE_DEBUG_ERRORTHROW

class BaseScriptVariable;

namespace DebugLog
{
	void Init();
	void Cleanup();
	void Push(std::string data, LogItem::Type type = LogItem::Type::Log);
	void WriteThread();
	void GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize);

#pragma region Scripting Bindings
	std::shared_ptr<BaseScriptVariable> Push(std::vector<std::shared_ptr<BaseScriptVariable>>&);
#pragma endregion
}