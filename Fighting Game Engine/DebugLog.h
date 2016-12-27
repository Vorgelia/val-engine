#pragma once
#include<string>
#include<vector>
namespace DebugLog{
	void Init();
	void Cleanup();
	void Push(std::string data,unsigned int stackSize=100);
	void WriteThread();
	void GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize);
}