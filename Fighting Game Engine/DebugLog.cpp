#include "DebugLog.h"
#include <windows.h>
#include "Time.h"
#include <thread>
#include <sstream>
#include <iostream>
#include <fstream>
#include <DbgHelp.h>
#include <queue>
#include <mutex>
#include "LogItem.h"
#include <stdexcept>
#include <atomic>

#include "BaseScriptVariable.h"
#include "ScriptError.h"
#include "Script.h"

namespace DebugLog
{
	//The thread containing the write loop.
	std::thread _writeThread;
	//Ofstream to log_output.txt
	std::ofstream _writeStream;
	//Queue of log items to write.
	std::queue<LogItem> _writeQueue;
	//Mutex that controls access to _writeQueue. We don't want both the main thread and _writeThread modifying information in it at the same time.
	std::mutex _queueMutex;
	//The function called with _writeThread runs on an infinite loop. _endWrite is a flag that tells that loop to end.
	std::atomic<bool> _endWrite = false;

	void Push(const std::string& data, LogItem::Type type = LogItem::Type::Log);
}

//Initialize the write stream and thread. Erase everything in log_output.txt and write a header in it.
void DebugLog::Init()
{
	_endWrite.store(false);
	_writeStream = std::ofstream("log_output.txt", std::ios::trunc);
	_writeStream << "--Val Engine Output Log--";
	_writeThread = std::thread(WriteThread);
}

//Raise the end write flag and wait until the write thread ends. Safely close the log_output.
void DebugLog::Cleanup()
{
	_endWrite.store(true);
	_writeThread.join();
	_writeStream.close();
}

//Infinite loop thread that goes through the write queue and prints its contents to a file, and the console.
void DebugLog::WriteThread()
{
	while(!_endWrite.load())
	{
		//If the write queue is empty, no reason to keep running this thread.
		if(_writeQueue.empty())
		{
			std::this_thread::yield();
			continue;
		}

		//Lock the queue mutex. This will not return until the queue mutex is unlocked, and will then lock it.
		//The queue mutex should be locked every time anything wants to access the write queue.
		_queueMutex.lock();
		LogItem li = _writeQueue.front();
		_writeQueue.pop();
		//Get stuff from the queue and then remove it. Then unlock the mutex, making it possible for other threads to access the queue again.
		_queueMutex.unlock();

		//Print different stuff to the console and the file based on the message type.

		int stackAmount = 0;
		std::string messagePrefix = "";

		switch(li.type())
		{
		case LogItem::Type::Message:
			stackAmount = 0;
			messagePrefix = "";
			break;
		case LogItem::Type::Error:
			stackAmount = -1;
			messagePrefix = "-Error";
			break;
		case LogItem::Type::Log:
			messagePrefix = "-Log";
			stackAmount = 1;
			break;
		case LogItem::Type::Warning:
			messagePrefix = "-Warning";
			stackAmount = 0;
			break;
		case LogItem::Type::ScriptLog:
			messagePrefix = "-Script Log";
			stackAmount = 2;
			break;
		}

		_writeStream << "\n\n" << messagePrefix << "\n" << li.ToString();
		std::clog << li.ToString(stackAmount) << std::endl << std::endl;
	}
}

//This will add data to the write queue. As before, the mutex here is to prevent the other thread from modifying the write queue as we're adding things to it.
//TODO: Port to ZMQ
void DebugLog::Push(const std::string& data, LogItem::Type type)
{
	_queueMutex.lock();
	_writeQueue.push(LogItem(data, type));
	GetStackTrace(_writeQueue.back().stack(), 100);
	_queueMutex.unlock();

#ifdef VE_DEBUG_ERRORTHROW
	if(type == LogItem::Type::Error)
	{
		throw std::runtime_error(data);
	}
#endif
}

void DebugLog::Push(const std::string & data, std::string fileName, int fileLine, LogItem::Type type)
{
	size_t lastSlash = fileName.find_last_of('\\') + 1;
	if(lastSlash == fileName.size())
	{
		Push(data, type);
		return;
	}

	fileName = fileName.substr(lastSlash, fileName.length() - lastSlash);
	Push("[" + fileName + ":" + std::to_string(fileLine) + "] " + data, type);
}

void DebugLog::GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize)
{
	//Copy-pasted from somewhere.
	//Get the call stack, run it against the debug symbols and get a bunch of text out of it.

	unsigned int   i;
	void          *stack[100];
	unsigned short frames;
	SYMBOL_INFO   *symbol;
	HANDLE         process;
	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(2, 100, stack, NULL);
	symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	storage->reserve(frames);
	frames = (frames - 5) > stackSize ? stackSize : (frames - 5);
	for(i = 0; i < frames; i++)
	{
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
		std::string symbol_name = "unknown symbol";
		if(!std::string(symbol->Name).empty())
			symbol_name = std::string(symbol->Name);
		storage->push_back(std::to_string(frames - i - 1) + ": " + symbol_name + " 0x" + std::to_string(symbol->Address));
	}
}

#pragma region Scripting Bindings

std::shared_ptr<BaseScriptVariable> DebugLog::Push(const Script* script, std::vector<std::shared_ptr<BaseScriptVariable>>& variables)
{
	std::stringstream str;

	str << "[Script: " << script->name() << "] ";

	for(const auto& var : variables)
	{
		if(var != nullptr)
		{
			str << var->ToString();
		}
	}

	DebugLog::Push(str.str(), LogItem::Type::ScriptLog);
	return std::make_shared<BaseScriptVariable>();
}

#pragma endregion