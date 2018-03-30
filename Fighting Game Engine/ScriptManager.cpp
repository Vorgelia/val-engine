#include "ScriptManager.h"
#include "Script.h"
#include "ServiceManager.h"
#include "BaseScriptVariable.h"
#include "ScriptVariable.h"
#include "ScriptCollection.h"
#include "FilesystemManager.h"
#include <memory>
#include "GameCharacter.h"
#include "CharacterStateManager.h"
#include "CharacterPhysicsManager.h"
#include "DebugLog.h"
#include "Time.h"

void ScriptManager::Init()
{
	_debug = _serviceManager->Debug();
	_time = _serviceManager->Time();
	_filesystem = _serviceManager->Filesystem();

	std::shared_ptr<ScriptCollection> collection = std::make_shared<ScriptCollection>();
	collection->AddMember("test", std::make_shared<ScriptInt>(5));
	_globalVariables.emplace(std::make_pair("testCollection", collection));
	HandleCharacterStateVariables();
	AddScript("Scripts/Base/example.vscript");
}

void ScriptManager::Update() {}

void ScriptManager::Cleanup()
{
	_scripts.clear();
	_globalVariables.clear();
}

Script* ScriptManager::GetScript(const FS::path & path)
{
	if(path.empty())
	{
		return nullptr;
	}

	return AddScript(path);
}

Script* ScriptManager::AddScript(const FS::path& path)
{
	std::vector<std::string> lines = _filesystem->ReturnFileLines(path, false);
	if(!lines.empty())
	{
		const std::string& scriptName = path.string();
		Script* script = _scripts.emplace(std::make_unique<Script>(scriptName, lines, _serviceManager)).first->get();

		HandleScriptBindings(script);
		script->Init();

		return script;
	}

	return nullptr;
}

void ScriptManager::HandleScriptBindings(Script* script)
{
	std::vector<std::string> &bindings = script->GetPragmaDirectives("Bind");

	for(const auto& directive : bindings)
	{
		if(directive == "UI")
		{
			//handle UI bindings
		}
		else if(directive == "Debug")
		{
			script->BindFunction("ve_log",
				[this](const Script* sc, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				return _debug->Log(sc, args);
			});
		}
		else if(directive == "Time")
		{
			script->BindFunction("time_frameCount",
				[this](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptInt>((long)_time->frameCount);
			});

			script->BindFunction("time_frameCountSinceLoad",
				[this](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptInt>((long)_time->frameCountSinceLoad);
			});
		}
	}
}

void ScriptManager::HandleCharacterStateVariables()
{
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_GENERIC", std::make_shared<ScriptInt>((int)CharacterStateFlagType::Generic)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_INVULN", std::make_shared<ScriptInt>((int)CharacterStateFlagType::Invuln)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_TARGET", std::make_shared<ScriptInt>((int)CharacterStateFlagType::CancelTargets)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_REQUIREMENT", std::make_shared<ScriptInt>((int)CharacterStateFlagType::CancelRequirements)));
}

void ScriptManager::AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable)
{
	_globalVariables.emplace(name, variable);
}

void ScriptManager::HandleScriptCharacterBindings(GameCharacter& character, Script* script) const
{
	script->BindFunction("character_startState",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(!args.empty() && args[0]->type() == ScriptVariableType::String)
			character.stateManager()->StartState(std::static_pointer_cast<ScriptString>(args[0])->value());
		return nullptr;
	});

	script->BindFunction("character_restartState",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateManager()->RestartState();
		return nullptr;
	});

	script->BindFunction("character_markStateEnded",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateManager()->MarkStateEnded();
		return nullptr;
	});

	script->BindFunction("character_setFrame",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(!args.empty() && args[0]->type() == ScriptVariableType::String)
			return std::make_shared<ScriptBool>(
				character.stateManager()->SetFrame(std::static_pointer_cast<ScriptString>(args[0])->value()));
		return nullptr;
	});

	script->BindFunction("character_allowStateSelfChaining",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(!args.empty() && args[0]->type() == ScriptVariableType::Bool)
			character.stateManager()->_allowStateSelfCancelling = std::static_pointer_cast<ScriptBool>(args[0])->value();
		return nullptr;
	});

	script->BindFunction("character_modifyStateFrame",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(!args.empty() && args[0]->type() == ScriptVariableType::Int)
			character.stateManager()->ModifyCurrentStateFrame(int(std::static_pointer_cast<ScriptInt>(args[0])->value()));
		return nullptr;
	});

	script->BindFunction("character_freeze",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(!args.empty() && args[0]->type() == ScriptVariableType::Int)
			character.stateManager()->Freeze(int(std::static_pointer_cast<ScriptInt>(args[0])->value()));
		return nullptr;
	});

	script->BindFunction("character_unfreeze",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateManager()->Unfreeze();
		return nullptr;
	});

	script->BindFunction("character_addFlag",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::String)
			character.stateManager()->AddFlag(
			(CharacterStateFlagType)std::static_pointer_cast<ScriptInt>(args[0])->value(),
				std::static_pointer_cast<ScriptString>(args[1])->value());
		return nullptr;
	});

	script->BindFunction("character_removeFlag",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::String)
			character.stateManager()->RemoveFlag(
			(CharacterStateFlagType)std::static_pointer_cast<ScriptInt>(args[0])->value(),
				std::static_pointer_cast<ScriptString>(args[1])->value());

		return nullptr;
	});

	script->BindFunction("character_clearFlags",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateManager()->ClearFlags();
		return nullptr;
	});

	script->BindFunction("character_addOffset",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->AddOffset(
				glm::lvec2(
					std::static_pointer_cast<ScriptInt>(args[0])->value(),
					std::static_pointer_cast<ScriptInt>(args[1])->value()
				));
		return nullptr;
	});

	script->BindFunction("character_addVelocity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->AddVelocity(
				glm::lvec2(
					std::static_pointer_cast<ScriptInt>(args[0])->value(),
					std::static_pointer_cast<ScriptInt>(args[1])->value()
				));
		return nullptr;
	});

	script->BindFunction("character_setVelocity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->SetVelocity(
				glm::lvec2(
					std::static_pointer_cast<ScriptInt>(args[0])->value(),
					std::static_pointer_cast<ScriptInt>(args[1])->value()
				));
		return nullptr;
	}); 

	script->BindFunction("character_getVelocity_x",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		return std::make_shared<ScriptInt>(character.physicsManager()->GetVelocity().x);
	});

	script->BindFunction("character_getVelocity_y",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		return std::make_shared<ScriptInt>(character.physicsManager()->GetVelocity().y);
	});

	script->BindFunction("character_addForce",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 3 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int && args[2]->type() == ScriptVariableType::Int)
			character.physicsManager()->AddForce(
				std::static_pointer_cast<ScriptInt>(args[0])->value(),
				glm::lvec2(
					std::static_pointer_cast<ScriptInt>(args[1])->value(),
					std::static_pointer_cast<ScriptInt>(args[2])->value()
				));
		return nullptr;
	});

	script->BindFunction("character_overrideGravity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->OverrideGravity(
				std::static_pointer_cast<ScriptInt>(args[0])->value(),
				std::static_pointer_cast<ScriptInt>(args[1])->value()
			);
		return nullptr;
	});

	script->BindFunction("character_overrideAirFriction",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->OverrideAirFriction(
				std::static_pointer_cast<ScriptInt>(args[0])->value(),
				std::static_pointer_cast<ScriptInt>(args[1])->value()
			);
		return nullptr;
	});

	script->BindFunction("character_overrideGroundFriction",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() == 2 && args[0]->type() == ScriptVariableType::Int && args[1]->type() == ScriptVariableType::Int)
			character.physicsManager()->OverrideGroundFriction(
				std::static_pointer_cast<ScriptInt>(args[0])->value(),
				std::static_pointer_cast<ScriptInt>(args[1])->value()
			);
		return nullptr;
	});
}

void ScriptManager::RemoveScript(Script* script)
{
	for(auto& iter : _scripts)
	{
		if(script == iter.get())
		{
			_scripts.erase(iter);
			return;
		}
	}
}

std::shared_ptr<BaseScriptVariable> ScriptManager::GetVariable(const std::string& name)
{
	auto& iter = _globalVariables.find(name);
	if(iter == _globalVariables.end())
	{
		return nullptr;
	}
	return iter->second;
}

ScriptManager::ScriptManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{

}

ScriptManager::~ScriptManager()
= default;
