#include "Profiler.h"
#include "Time.h"
#include "DebugLog.h"
//This profiler implementation is fairly simple. Just check the time that passed between profiler calls.
//Structure modelled after Unity's Profiler.BeginSample and Profiler.EndSample.
namespace Profiler
{
	std::unordered_map<std::string, double> _elements;
	glm::vec2 _stateChanges;
}

void Profiler::Begin(const std::string& str)
{
	_elements[str] = glfwGetTime();
}

void Profiler::End(const std::string& str)
{
	//glFinish();
	_elements[str] = glfwGetTime() - _elements[str];
}

std::string Profiler::String(const std::string& str)
{
	return "Profiler| " + str + ":  " + std::to_string(_elements[str] * 1000) + "ms";
}

void Profiler::Clear()
{
	//elements.clear();
	_stateChanges = glm::vec2(0, 0);
}

void Profiler::Print()
{
	for(auto i = _elements.begin(); i != _elements.end(); ++i)
		DebugLog::Push(Profiler::String(i->first), LogItem::Type::Message);
}