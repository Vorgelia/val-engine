#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stack>

enum class ScriptExitCode;
enum class ScriptControlFlag;
class ScriptParentBlock;
class ScriptVariable;

class Script
{
	friend class ScriptBlock;
	friend class ScriptParentBlock;

	std::string _name;
	bool _valid;

	std::vector<std::string> _lines;
	std::map<std::string, std::string> _pragmaDirectives;

	std::map<std::string, void(*)()> _boundFunctions;

	std::stack<ScriptBlock*> _blockStack;
	ScriptControlFlag _controlFlag;

	ScriptParentBlock* _parentBlock;

	void PreProcess();

	void PushBlock(ScriptBlock* block);
	void PopBlock();

public:
	std::string name() const;
	bool valid() const;

	ScriptControlFlag controlFlag();

	void BindFunction(std::string name, void(*func)());
	bool CallBoundFunction(std::string name);

	void RaiseControlFlag(ScriptControlFlag flag);
	void ConsumeControlFlag();

	ScriptExitCode Execute();
	void ExecuteFunction(std::string name, const std::vector<ScriptVariable> &variables);

	Script(std::string name, std::vector<std::string> lines);
	~Script();
};

