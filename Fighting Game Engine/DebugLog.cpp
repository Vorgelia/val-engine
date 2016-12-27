#include "DebugLog.h"
#include "Time.h"
#include <thread>
#include <fstream>
#include <DbgHelp.h>
#include <queue>
#include <mutex>
#include "LogItem.h"

namespace DebugLog{
	std::thread _writeThread;
	std::ofstream _writeStream;
	std::queue<LogItem> _writeQueue;
	std::mutex _queueMutex;

	bool _endWrite=false;
}

void DebugLog::Init(){
	_endWrite = false;
	_writeStream = std::ofstream("error_output.txt",std::ios::trunc);
	_writeQueue.push(LogItem("--Val Engine Error Log--\n"));
	_writeThread = std::thread(WriteThread);
}
void DebugLog::Cleanup(){
	_endWrite = true;
	_writeThread.join();
	_writeStream.close();
}

void DebugLog::WriteThread(){
	while (!_endWrite){
		if (_writeQueue.empty())
			continue;
		_queueMutex.lock();
		LogItem li=_writeQueue.front();
		_writeQueue.pop();
		_queueMutex.unlock();
		_writeStream << "\nLog:"+li.ToString();
		std::clog << li.ToString(1) << std::endl;
	}
}

void DebugLog::Push(std::string data, unsigned int stackSize){
	_queueMutex.lock();
	_writeQueue.push(LogItem(data));
	GetStackTrace(&_writeQueue.back().stack,stackSize);
	_queueMutex.unlock();
}


void DebugLog::GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize){
	//Copy-pasted from somewhere
	std::string returnString;
	int   i;
	void          *stack[100];
	short frames;
	SYMBOL_INFO   *symbol;
	HANDLE         process;
	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(2, 100, stack, NULL);
	symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO)+256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	storage->reserve(frames);
	frames = frames>stackSize?stackSize:frames;
	for (i = 0; i < frames; i++)
	{
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
		std::string symbol_name = "unknown symbol";
		if (!std::string(symbol->Name).empty())
			symbol_name = std::string(symbol->Name);
		storage->push_back(std::to_string(frames - i - 1) + ": " + symbol_name + " 0x" + std::to_string(symbol->Address));
	}
}