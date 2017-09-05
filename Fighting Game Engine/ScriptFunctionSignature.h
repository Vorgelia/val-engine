#pragma once
#include <string>
#include <vector>

struct ScriptFunctionSignature
{
public:
	std::string name;

	int start;
	int end;

	std::vector<std::string> arguments;
	std::string returnType;

	ScriptFunctionSignature(std::string name, int start, int end, std::vector<std::string> arguments, std::string returnType);
	ScriptFunctionSignature();
	~ScriptFunctionSignature();
};
