#include "ScriptManager.h"
#include"Script.h"
#include "ResourceLoader.h"

namespace ScriptManager
{
	std::vector<Script*> _scripts;
}

void ScriptManager::Init()
{
	AddScript("Scripts/Base/example.vscript");
}

void ScriptManager::AddScript(FS::path path)
{
	std::vector<std::string> lines = ResourceLoader::ReturnFileLines(path, false);
	if(lines.size() > 0)
	{
		_scripts.push_back(new Script(path.leaf().generic_string(), lines));
	}
}

void ScriptManager::Update()
{
	for(unsigned int i = 0; i < _scripts.size(); ++i)
	{
		if(_scripts[i]->valid())
		{
			_scripts[i]->Execute();
		}
	}
}

void ScriptManager::Cleanup()
{
	for(unsigned int i = 0; i < _scripts.size(); ++i)
	{
		delete _scripts[i];
	}

	_scripts.clear();
}