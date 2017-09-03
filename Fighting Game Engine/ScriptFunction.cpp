#include "ScriptFunction.h"


ScriptFunction::ScriptFunction(std::string name, int start, int end, std::vector<std::string> arguments)
{
	this->name = name;
	this->start = start;
	this->end = end;
	this->arguments = arguments;
}

ScriptFunction::~ScriptFunction()
{
}
