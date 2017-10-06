#pragma once
#include <stdexcept>
#include <string>

enum class ScriptExitCode
{
	Success = 0,
	Failure = 1,
	Exception = 2,
};

class ScriptError : public std::logic_error 
{
public:
	ScriptError(std::string message);
};
