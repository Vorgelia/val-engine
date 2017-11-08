#pragma once
#include <stdexcept>
#include <string>

class ScriptError : public std::logic_error 
{
public:
	ScriptError(const std::string& message);
};
