#pragma once
#include "BaseService.h"
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <fstream>
#include <atomic>
#include <thread>
#include "LogItem.h"

#define VE_DEBUG_ERRORTHROW

#define VE_LOG(data, ...)\
	Log(data, __FILE__, __LINE__, ##__VA_ARGS__)

class BaseScriptVariable;
class Script;

class Debug : public BaseService
{
	VE_OBJECT_DECLARATION(Debug, BaseService);

private:
	//The thread containing the write loop.
	std::thread _writeThread;
	//Ofstream to log_output.txt
	std::ofstream _writeStream;
	//Queue of log items to write.
	std::queue<LogItem> _writeQueue;
	//Mutex that controls access to _writeQueue. We don't want both the main thread and _writeThread modifying information in it at the same time.
	std::mutex _queueMutex;
	//The function called with _writeThread runs on an infinite loop. _endWrite is a flag that tells that loop to end.
	std::atomic<bool> _endWrite;

	void Log(const std::string& data, LogItem::Type type = LogItem::Type::Log);

public:
	void OnInit() override;
	void OnServiceInit() override;
	void OnServiceCleanup() override;

	void Log(const std::string& data, std::string fileName, int fileLine, LogItem::Type type = LogItem::Type::Log);
	std::shared_ptr<BaseScriptVariable> Log(const Script* script, std::vector<std::shared_ptr<BaseScriptVariable>>&);
	void WriteThread();
	void GetStackTrace(std::vector<std::string>* storage, unsigned int stackSize) const;


	Debug() = default;
	virtual ~Debug() = default;
};