#include "Profiler.h"
#include "Time.h"
#include "DebugLog.h"
//This profiler implementation is fairly simple. Just check the time that passed between profiler calls.
//Structure modelled after Unity's Profiler.BeginSample and Profiler.EndSample.
namespace Profiler{
	std::unordered_map<std::string, double> elements;
	glm::vec2 stateChanges;
}
void Profiler::Begin(std::string str){
	elements[str] = glfwGetTime();
}
void Profiler::End(std::string str){
//	glFinish();
	elements[str] = glfwGetTime() - elements[str];
}
std::string Profiler::String(std::string str){
	return "Profiler| " + str +":  "+std::to_string(elements[str]*1000)+"ms";
}
void Profiler::Clear(){
	//elements.clear();
	stateChanges = glm::vec2(0, 0);
}
void Profiler::Print(){
	for (auto i = elements.begin(); i != elements.end(); ++i)
		DebugLog::Push(Profiler::String(i->first),0);
}