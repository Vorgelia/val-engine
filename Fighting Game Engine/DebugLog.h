#pragma once
#include<string>
#include<vector>
#include"LogItem.h"

#define VE_DEBUG_ERRORTHROW

namespace DebugLog{
	void Init();
	void Cleanup();
	void Push(std::string data, LogItem::Type type = LogItem::Type::Log);
	void WriteThread();
	void GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize);
}