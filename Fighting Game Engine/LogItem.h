#pragma once
#include<string>
#include<vector>

//Very simple container class for log data.
class LogItem
{
public:
	enum class Type
	{
		Log,
		Warning,
		Error,
		Message,
		ScriptLog
	};

private:
	std::string _message;
	LogItem::Type _type;
	std::vector<std::string> _stack;
public:

	std::string message();
	LogItem::Type type();
	std::vector<std::string>* stack();

	std::string ToString(unsigned int stackAmount = 100);

	LogItem(const std::string& msg, std::vector<std::string>& stack, LogItem::Type type = LogItem::Type::Log);
	LogItem(const std::string& msg, LogItem::Type type = LogItem::Type::Log);
};
