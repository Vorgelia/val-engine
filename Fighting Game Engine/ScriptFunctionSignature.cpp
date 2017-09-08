#include "ScriptFunctionSignature.h"

ScriptFunctionSignature::ScriptFunctionSignature(std::string name, int start, int end, std::vector<std::string> arguments, std::string returnType)
{
	this->name = name;
	this->start = start;
	this->end = end;
	this->arguments = arguments;
	this->returnType = returnType;
}

ScriptFunctionSignature::ScriptFunctionSignature()
{
}

ScriptFunctionSignature::~ScriptFunctionSignature()
{
}
