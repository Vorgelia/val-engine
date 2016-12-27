#include "LogItem.h"


std::string LogItem::ToString(int stackAmount){
	std::string returnString = message;
	stackAmount = stackAmount>stack.size()?stack.size():stackAmount;
	for (unsigned int i = 0; i < stackAmount; ++i){
		returnString += "\n\t" + stack[i];
	}
	return returnString;
}

LogItem::LogItem(std::string msg, std::vector<std::string> stack){
	message = msg;
	this->stack = stack;
}
LogItem::LogItem(std::string msg){
	message = msg;
}
