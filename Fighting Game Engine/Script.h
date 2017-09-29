#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stack>

enum class ScriptExitCode;
enum class ScriptControlFlag;
class ScriptBlock;
class ScriptParentBlock;
class BaseScriptVariable;

class Script
{
	friend class ScriptBlock;
	friend class ScriptParentBlock;

	std::string _name;
	bool _valid;

	std::vector<std::string> _lines;
	std::map<std::string, std::string> _pragmaDirectives;

	std::map<std::string, void(*)(std::vector<std::shared_ptr<BaseScriptVariable>>&)> _boundFunctions;

	std::stack<std::shared_ptr<ScriptBlock>> _blockStack;
	ScriptControlFlag _controlFlag;

	ScriptParentBlock* _parentBlock;

	void PreProcess();

	void PushBlock(std::shared_ptr<ScriptBlock> block);
	void PopBlock();

public:
	std::string name() const;
	bool valid() const;

	ScriptControlFlag controlFlag();

	void BindFunction(std::string name, void(*func)(std::vector<std::shared_ptr<BaseScriptVariable>>&));
	bool CallBoundFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	std::shared_ptr<BaseScriptVariable> GetGlobalVariable(const std::string& name) const;

	void RaiseControlFlag(ScriptControlFlag flag);
	void ConsumeControlFlag();

	ScriptExitCode Execute();
	void ExecuteFunction(std::string name, const std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	Script(std::string name, std::vector<std::string> lines);
	~Script();
};