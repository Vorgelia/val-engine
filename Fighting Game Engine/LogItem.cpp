#include "LogItem.h"

std::string LogItem::message()
{
	return _message;
}

LogItem::Type LogItem::type()
{
	return _type;
}

std::vector<std::string>* LogItem::stack()
{
	return &_stack;
}

std::string LogItem::ToString(unsigned int stackAmount)
{
	std::string returnString = _message;
	stackAmount = stackAmount > _stack.size() ? _stack.size() : stackAmount;

	for(unsigned int i = 0; i < stackAmount; ++i)
	{
		returnString += "\n\t" + _stack[i];
	}

	return returnString;
}

LogItem::LogItem(const std::string& msg, std::vector<std::string>& stack, LogItem::Type type)
{
	_message = msg;
	_stack = stack;
	_type = type;
}
LogItem::LogItem(const std::string& msg, LogItem::Type type)
{
	_message = msg;
	_type = type;
}
