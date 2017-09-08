#pragma once
#include <stdexcept>
#include <string>

enum class ScriptExitCode
{
	Success = 0,
	Failure = 1,
	Exception = 2,
};

class ScriptError : std::logic_error
{
	int _line;

	std::string _scriptName;
public:
	const char* what() const;
	ScriptError(std::string message, std::string scriptName, int line);
	ScriptError(std::string message);
};
