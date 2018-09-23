#include "ScriptFunctionSignature.h"
#include "BaseScriptVariable.h"

ScriptFunctionSignature::ScriptFunctionSignature(const std::string& name, int start, int end, std::vector<ScriptVariableSignature>& arguments, ScriptVariableType returnType)
{
	this->name = name;
	this->start = start;
	this->end = end;
	this->arguments = arguments;
	this->returnType = returnType;
}

ScriptVariableSignature::ScriptVariableSignature(ScriptVariableType type, const std::string& name)
{
	this->type = type;
	this->name = name;
}

ScriptFunctionSignature::ScriptFunctionSignature()
= default;

ScriptFunctionSignature::~ScriptFunctionSignature()
= default;
