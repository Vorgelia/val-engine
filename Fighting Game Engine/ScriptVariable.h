#pragma once
#include<string>

class ScriptVariable
{
	std::string _type;
	std::string _value;
public:
	ScriptVariable(std::string type);
	ScriptVariable(std::string type, std::string value);
	ScriptVariable();
	~ScriptVariable();
};

