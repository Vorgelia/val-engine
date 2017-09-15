#include "ScriptManager.h"
#include"Script.h"
#include "ResourceLoader.h"

namespace ScriptManager
{
	std::vector<Script*> scripts;
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
		scripts.push_back(new Script(path.leaf().generic_string(), lines));
	}
}

void ScriptManager::Update()
{
	for(unsigned int i = 0; i < scripts.size(); ++i)
	{
		if(scripts[i]->valid())
		{
			scripts[i]->Execute();
		}
	}
}

void ScriptManager::Cleanup()
{
	for(unsigned int i = 0; i < scripts.size(); ++i)
	{
		delete scripts[i];
	}

	scripts.clear();
}