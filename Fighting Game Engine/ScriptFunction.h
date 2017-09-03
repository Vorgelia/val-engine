#pragma once
#include <string>
#include <vector>

//Closer to a function signature
struct ScriptFunction
{
public:
	std::string name;

	int start;
	int end;

	std::vector<std::string> arguments;

	ScriptFunction(std::string name, int start, int end, std::vector<std::string> arguments);
	~ScriptFunction();
};
