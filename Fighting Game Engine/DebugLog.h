#pragma once
#include <string>
#include <vector>
#include <memory>
#include "LogItem.h"

#define VE_DEBUG_ERRORTHROW

#define VE_DEBUG_LOG(data, ...)\
	DebugLog::Push(data, __FILE__, __LINE__, ##__VA_ARGS__)

class BaseScriptVariable;
class Script;

namespace DebugLog
{
	void Init();
	void Cleanup();
	void Push(const std::string& data, std::string fileName, int fileLine, LogItem::Type type = LogItem::Type::Log);
	void WriteThread();
	void GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize);

#pragma region Scripting Bindings
	std::shared_ptr<BaseScriptVariable> Push(const Script* script, std::vector<std::shared_ptr<BaseScriptVariable>>&);
#pragma endregion
}