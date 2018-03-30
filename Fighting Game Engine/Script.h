#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <functional>

enum class ScriptExitCode;
enum class ScriptControlFlag;
struct ScriptLine;
class ScriptBlock;
class ScriptParentBlock;
class BaseScriptVariable;
class ServiceManager;
class Debug;
class ScriptManager;

typedef std::vector<std::shared_ptr<BaseScriptVariable>> ScriptArgumentCollection;

//TODO: Get rid of shared_ptr
class Script
{
	friend class ScriptBlock;
	friend class ScriptParentBlock;

private:
	ServiceManager* _serviceManager;

	Debug* _debug;
	ScriptManager* _scriptManager;

private:
	std::string _name;
	bool _valid;

	//TODO: Line token/number caching
	std::vector<std::string> _rawLines;
	std::vector<ScriptLine> _lines;

	std::unordered_map<std::string, std::vector<std::string>> _pragmaDirectives;

	std::unordered_map<std::string, std::function<std::shared_ptr<BaseScriptVariable>(const Script*, ScriptArgumentCollection&)>> _boundFunctions;

	std::stack<std::shared_ptr<ScriptBlock>> _blockStack;
	ScriptControlFlag _controlFlag;

	std::shared_ptr<ScriptParentBlock> _parentBlock;

	void PreProcess();

	void PushBlock(std::shared_ptr<ScriptBlock> block);
	void PopBlock();

	std::shared_ptr<BaseScriptVariable> GetGlobalVariable(const std::string& name) const;
public:
	std::string name() const;
	bool valid() const;

	ScriptControlFlag controlFlag() const;

	bool HasFunction(const std::string& name) const;

	void BindFunction(const std::string& name, std::function<std::shared_ptr<BaseScriptVariable>(const Script*, ScriptArgumentCollection&)> func);
	std::shared_ptr<BaseScriptVariable> CallBoundFunction(const std::string& name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	std::vector<std::string> GetPragmaDirectives(const std::string& id);
	std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name) const;

	void RaiseControlFlag(ScriptControlFlag flag);
	void ConsumeControlFlag();

	void Init();
	void Execute();
	void ExecuteFunction(const std::string& name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables = std::vector<std::shared_ptr<BaseScriptVariable>>());

	Script(const std::string& name, std::vector<std::string> lines, ServiceManager* serviceManager);
	~Script();
};