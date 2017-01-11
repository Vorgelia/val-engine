#pragma once
#include<string>
#include<vector>
class LogItem{
public:
	enum class Type{
		Log,
		Warning,
		Error,
		Message
	};
	std::string message;
	LogItem::Type type;
	std::vector<std::string> stack;
	std::string ToString(unsigned int stackAmount = 100);
	LogItem(std::string msg, std::vector<std::string> stack, LogItem::Type type = LogItem::Type::Log);
	LogItem(std::string msg, LogItem::Type type = LogItem::Type::Log);
};
