#pragma once
#include<boost\filesystem.hpp>
namespace FS = boost::filesystem;

class Script;

namespace ScriptManager
{
	void Init();

	void AddScript(FS::path path);
	void Update();
	void Cleanup();
}
