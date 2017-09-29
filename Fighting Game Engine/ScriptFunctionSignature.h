#pragma once
#include <string>
#include <vector>

enum class ScriptVariableType;

struct ScriptFunctionSignature
{
	std::string name;

	int start;
	int end;

	std::vector<std::string> arguments;
	ScriptVariableType returnType;

	ScriptFunctionSignature(std::string name, int start, int end, std::vector<std::string> arguments, ScriptVariableType returnType);
	ScriptFunctionSignature();
	~ScriptFunctionSignature();
};
