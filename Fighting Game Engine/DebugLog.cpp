#include "DebugLog.h"
#include <windows.h>
#include "Time.h"
#include <sstream>
#include <DbgHelp.h>
#include "LogItem.h"
#include <stdexcept>

#include "BaseScriptVariable.h"
#include "ScriptError.h"
#include "Script.h"

#include "ServiceManager.h"

void Debug::Update() {}

void Debug::Init()
{
}

//Infinite loop thread that goes through the write queue and prints its contents to a file, and the console.
void Debug::WriteThread()
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
void Debug::Log(const std::string& data, LogItem::Type type)
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

void Debug::Log(const std::string& data, std::string fileName, int fileLine, LogItem::Type type)
{
	size_t lastSlash = fileName.find_last_of('\\') + 1;
	if(lastSlash == fileName.size())
	{
		Log(data, type);
		return;
	}

	fileName = fileName.substr(lastSlash, fileName.length() - lastSlash);
	Log("[" + fileName + ":" + std::to_string(fileLine) + "] " + data, type);
}

void Debug::GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize)
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
	frames = (frames - 5) > (int)stackSize ? (int)stackSize : (frames - 5);
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

std::shared_ptr<BaseScriptVariable> Debug::Log(const Script* script, std::vector<std::shared_ptr<BaseScriptVariable>>& variables)
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

	Log(str.str(), LogItem::Type::ScriptLog);
	return std::make_shared<BaseScriptVariable>();
}

Debug::Debug(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	_endWrite.store(false);
	_writeStream = std::ofstream("log_output.txt", std::ios::trunc);
	_writeStream << "--Val Engine Output Log--";
	_writeThread = std::thread([this]() { WriteThread(); });
}

Debug::~Debug()
{
	_endWrite.store(true);
	_writeThread.join();
	_writeStream.close();
}

#pragma endregion