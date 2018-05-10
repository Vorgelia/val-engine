#include "ScriptManager.h"
#include "Script.h"
#include "ServiceManager.h"
#include "BaseScriptVariable.h"
#include "ScriptVariable.h"
#include "ScriptCollection.h"
#include "FilesystemManager.h"
#include <memory>
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterPhysicsComponent.h"
#include "DebugLog.h"
#include "Time.h"
#include <boost/algorithm/string.hpp>
#include "ResourceManager.h"

#define GET_ARG_DEC_CHECKED(collection, index, storage)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::Dec)\
			return nullptr;\
		ve::dec_t storage = std::static_pointer_cast<ScriptDec>((collection)[index])->value();

#define GET_ARG_STRING_CHECKED(collection, index, storage)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::String)\
			return nullptr;\
		std::string storage = std::static_pointer_cast<ScriptString>((collection)[index])->value();

#define GET_ARG_BOOL_CHECKED(collection, index, storage)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::Bool)\
			return nullptr;\
		bool (storage) = std::static_pointer_cast<ScriptBool>((collection)[index])->value();

#define GET_ARG_COLLECTION_CHECKED(collection, index, storage)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::Collection)\
			return nullptr;\
		ScriptCollection& (storage) = *std::static_pointer_cast<ScriptCollection>((collection)[index]);

#define GET_ARG_VAR_CHECKED(collection, index, storage)\
		if((index) == 0 ? (collection).empty() : (collection).size() <= (index))\
			return nullptr;\
		std::shared_ptr<BaseScriptVariable> (storage) = (collection)[index];

void ScriptManager::Init()
{
	_debug = _serviceManager->Debug();
	_time = _serviceManager->Time();
	_filesystem = _serviceManager->Filesystem();
	_resource = _serviceManager->ResourceManager();

	CacheGlobalVariables();
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
	const std::string* scriptSource = _resource->GetTextData(path.string());
	if(scriptSource == nullptr)
	{
		return nullptr;
	}

	std::vector<std::string> lines;
	boost::split(lines, *scriptSource, boost::is_any_of(std::string("\n")), boost::token_compress_on);

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
				return std::make_shared<ScriptDec>(long(_time->frameCount));
			});

			script->BindFunction("time_frameCountSinceLoad",
				[this](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptDec>(long(_time->frameCountSinceLoad));
			});
		}
	}

	script->BindFunction("collection_add",
		[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_COLLECTION_CHECKED(args, 0, collection);
		GET_ARG_STRING_CHECKED(args, 1, name);
		GET_ARG_VAR_CHECKED(args, 2, variable);
		collection.AddMember(name, variable);
		return nullptr;
	});

	script->BindFunction("collection_remove",
		[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_COLLECTION_CHECKED(args, 0, collection);
		GET_ARG_STRING_CHECKED(args, 1, name);
		collection.RemoveMember(name);
		return nullptr;
	});

	//TODO: Fixed64 Math functions
}

void ScriptManager::CacheGlobalVariables()
{
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_GENERIC", std::make_shared<ScriptDec>((int)CharacterStateFlagType::Generic)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_INVULN", std::make_shared<ScriptDec>((int)CharacterStateFlagType::Invuln)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_TARGET", std::make_shared<ScriptDec>((int)CharacterStateFlagType::CancelTargets)));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_REQUIREMENT", std::make_shared<ScriptDec>((int)CharacterStateFlagType::CancelRequirements)));

	std::shared_ptr<ScriptCollection> collection = std::make_shared<ScriptCollection>();
	collection->AddMember("test", std::make_shared<ScriptDec>(5));
	_globalVariables.emplace(std::make_pair("testCollection", collection));
}

void ScriptManager::AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable)
{
	_globalVariables.emplace(name, variable);
}

void ScriptManager::HandleScriptCharacterBindings(GameCharacter& character, Script* script) const
{
	script->BindFunction("character_util_runCharacterScriptFunction",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		if(args.size() <= 1)
		{
			return nullptr;
		}

		GET_ARG_STRING_CHECKED(args, 0, functionName);
		ScriptArgumentCollection parentFunctionArgs = ScriptArgumentCollection(args.begin() + 1, args.end());
		character._characterScript->ExecuteFunction(functionName, parentFunctionArgs);
		return nullptr;
	});

	script->BindFunction("character_state_startState",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_STRING_CHECKED(args, 0, state);
		character.stateComponent()->StartState(state);
		return nullptr;
	});

	script->BindFunction("character_state_restartState",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateComponent()->RestartState();
		return nullptr;
	});

	script->BindFunction("character_state_markStateEnded",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateComponent()->MarkStateEnded();
		return nullptr;
	});

	script->BindFunction("character_state_setFrame",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_STRING_CHECKED(args, 0, frame);
		return std::make_shared<ScriptBool>(character.stateComponent()->SetFrame(frame));
	});

	script->BindFunction("character_state_allowStateSelfChaining",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_BOOL_CHECKED(args, 0, allow);
		character.stateComponent()->_allowStateSelfCancelling = allow;
		return nullptr;
	});

	script->BindFunction("character_state_modifyStateFrame",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, frame);
		character.stateComponent()->ModifyCurrentStateFrame(int(frame));
		return nullptr;
	});

	script->BindFunction("character_state_freeze",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, time);
		character.stateComponent()->Freeze(int(time));
		return nullptr;
	});

	script->BindFunction("character_state_unfreeze",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateComponent()->Unfreeze();
		return nullptr;
	});

	script->BindFunction("character_state_addFlag",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{

		GET_ARG_DEC_CHECKED(args, 0, flagType);
		GET_ARG_STRING_CHECKED(args, 1, flag);
		character.stateComponent()->AddFlag(CharacterStateFlagType(int(flagType)), flag);
		return nullptr;
	});

	script->BindFunction("character_state_removeFlag",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, flagType);
		GET_ARG_STRING_CHECKED(args, 1, flag);
		character.stateComponent()->RemoveFlag(CharacterStateFlagType(int(flagType)), flag);
		return nullptr;
	});

	script->BindFunction("character_state_clearFlags",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		character.stateComponent()->ClearFlags();
		return nullptr;
	});

	script->BindFunction("character_physics_addOffset",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, x);
		GET_ARG_DEC_CHECKED(args, 1, y);
		character.physicsComponent()->AddOffset(ve::vec2(x, y));
		return nullptr;
	});

	script->BindFunction("character_physics_addVelocity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, x);
		GET_ARG_DEC_CHECKED(args, 1, y);
		character.physicsComponent()->AddVelocity(ve::vec2(x, y));
		return nullptr;
	});

	script->BindFunction("character_physics_setVelocity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, x);
		GET_ARG_DEC_CHECKED(args, 1, y);
		character.physicsComponent()->SetVelocity(ve::vec2(x, y));
		return nullptr;
	});

	script->BindFunction("character_physics_getVelocity_x",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		return std::make_shared<ScriptDec>(character.physicsComponent()->GetVelocity().x);
	});

	script->BindFunction("character_physics_getVelocity_y",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		return std::make_shared<ScriptDec>(character.physicsComponent()->GetVelocity().y);
	});

	script->BindFunction("character_physics_addForce",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, duration);
		GET_ARG_DEC_CHECKED(args, 1, x);
		GET_ARG_DEC_CHECKED(args, 2, y);
		character.physicsComponent()->AddForce(ve::int_t(duration), ve::vec2(x, y));
		return nullptr;
	});

	script->BindFunction("character_physics_overrideGravity",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, duration);
		GET_ARG_DEC_CHECKED(args, 1, value);
		character.physicsComponent()->OverrideGravity(ve::int_t(duration), value);
		return nullptr;
	});

	script->BindFunction("character_physics_overrideAirFriction",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, duration);
		GET_ARG_DEC_CHECKED(args, 1, value);
		character.physicsComponent()->OverrideAirFriction(ve::int_t(duration), value);
		return nullptr;
	});

	script->BindFunction("character_physics_overrideGroundFriction",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_DEC_CHECKED(args, 0, duration);
		GET_ARG_DEC_CHECKED(args, 1, value);
		character.physicsComponent()->OverrideGroundFriction(ve::int_t(duration), value);
		return nullptr;
	});

	script->AddVariable("character_objectId", std::make_shared<ScriptDec>(character.object()->id(), true));
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
