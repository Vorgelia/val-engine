#include "LogItem.h"


std::string LogItem::ToString(unsigned int stackAmount)
{
	std::string returnString = message;
	stackAmount = stackAmount > stack.size() ? stack.size() : stackAmount;
	for(unsigned int i = 0; i < stackAmount; ++i)
	{
		returnString += "\n\t" + stack[i];
	}
	return returnString;
}

LogItem::LogItem(std::string msg, std::vector<std::string> stack, LogItem::Type type)
{
	this->message = msg;
	this->stack = stack;
	this->type = type;
}
LogItem::LogItem(std::string msg, LogItem::Type type)
{
	this->message = msg;
	this->type = type;
}
