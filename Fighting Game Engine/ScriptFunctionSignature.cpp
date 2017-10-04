#include "ScriptFunctionSignature.h"
#include "BaseScriptVariable.h"

ScriptFunctionSignature::ScriptFunctionSignature(std::string name, int start, int end, std::vector<ScriptVariableSignature> arguments, ScriptVariableType returnType)
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

ScriptVariableSignature::ScriptVariableSignature(ScriptVariableType type, std::string name)
{
	this->type = type;
	this->name == name;
}
