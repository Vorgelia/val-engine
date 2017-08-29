#pragma once
#include <stdexcept>

enum class ScriptExitCode
{
	Success = 0,
	Failure = 1,
	Exception = 2,
};

typedef std::logic_error ScriptError;
