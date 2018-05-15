#include "ScriptManager.h"
#include "Script.h"
#include "ServiceManager.h"
#include "BaseScriptVariable.h"
#include "ScriptVariable.h"
#include "ScriptMap.h"
#include "ScriptArray.h"
#include "FilesystemManager.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterPhysicsComponent.h"
#include "DebugLog.h"
#include "Time.h"
#include "ResourceManager.h"

#include <memory>
#include <boost/algorithm/string.hpp>

#define GET_ARG_VALUE_CHECKED(collection, index, storage, varType, conversionType)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::varType)\
			return nullptr;\
		conversionType::value_type storage = std::static_pointer_cast<conversionType>((collection)[index])->value();

#define GET_ARG_CHECKED(collection, index, storage, varType, conversionType)\
		if(((index) == 0 ? (collection).empty() : (collection).size() <= (index)) || (collection)[index]->type() != ScriptVariableType::varType)\
			return nullptr;\
		std::shared_ptr<conversionType> storage = std::static_pointer_cast<conversionType>((collection)[index]);
	
#define GET_ARG_VAR_CHECKED(collection, index, storage)\
		if((index) == 0 ? (collection).empty() : (collection).size() <= (index))\
			return nullptr;\
		std::shared_ptr<BaseScriptVariable> (storage) = (collection)[index];

#define GET_ARG_DEC_CHECKED(collection, index, storage) GET_ARG_VALUE_CHECKED(collection, index, storage, Dec, ScriptDec)
#define GET_ARG_STRING_CHECKED(collection, index, storage) GET_ARG_VALUE_CHECKED(collection, index, storage, String, ScriptString)
#define GET_ARG_BOOL_CHECKED(collection, index, storage) GET_ARG_VALUE_CHECKED(collection, index, storage, Bool, ScriptBool)
#define GET_ARG_MAP_CHECKED(collection, index, storage) GET_ARG_CHECKED(collection, index, storage, Map, ScriptMap)
#define GET_ARG_ARRAY_CHECKED(collection, index, storage) GET_ARG_CHECKED(collection, index, storage, Array, ScriptArray)

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
	std::vector<std::string>& bindings = script->GetPragmaDirectives("Bind");
	std::unordered_set<std::string> handledBindings;

	for(const auto& directive : bindings)
	{
		if(handledBindings.find(directive) != handledBindings.end())
		{
			continue;
		}
		handledBindings.emplace(directive);

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
			script->BindFunction("ve_time_frameCount",
				[this](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptDec>(ve::dec_t(_time->frameCount));
			});

			script->BindFunction("ve_time_frameCountSinceLoad",
				[this](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptDec>(ve::dec_t(_time->frameCountSinceLoad));
			});
		}
		else if(directive == "Map")
		{
			script->BindFunction("ve_map_add",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_MAP_CHECKED(args, 0, collection);
				GET_ARG_CHECKED(args, 1, name, String, ScriptString);
				GET_ARG_VAR_CHECKED(args, 2, variable);
				return collection->AddMember(name, variable);
			});

			script->BindFunction("ve_map_remove",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_MAP_CHECKED(args, 0, collection);
				GET_ARG_CHECKED(args, 1, name, String, ScriptString);
				collection->RemoveMember(name);
				return nullptr;
			});

			script->BindFunction("ve_map_clear",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_MAP_CHECKED(args, 0, collection);
				collection->Clear();
				return nullptr;
			});
		}
		else if(directive == "Array")
		{
			script->BindFunction("ve_array_push",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{

				GET_ARG_ARRAY_CHECKED(args, 0, collection);
				for(size_t i = 1 ; i < args.size(); ++i)
				{
					collection->Push(args[i]);
				}
				
				return collection->Back();
			});

			script->BindFunction("ve_array_pop",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_ARRAY_CHECKED(args, 0, collection);
				return collection->Pop();
			});

			script->BindFunction("ve_array_remove",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_ARRAY_CHECKED(args, 0, collection);
				GET_ARG_CHECKED(args, 1, index, Dec, ScriptDec);
				collection->RemoveMember(index);
				return nullptr;
			});

			script->BindFunction("ve_array_size",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_ARRAY_CHECKED(args, 0, collection);
				return collection->Size();
			});

			script->BindFunction("ve_array_clear",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_ARRAY_CHECKED(args, 0, collection);
				collection->Clear();
				return nullptr;
			});
		}
		else if(directive == "Math")
		{
			script->BindFunction("ve_math_sin",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Sin(dec));
			});

			script->BindFunction("ve_math_cos",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Cos(dec));
			});

			script->BindFunction("ve_math_sqrt",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Sqrt(dec));
			});

			script->BindFunction("ve_math_rsqrt",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::RSqrt(dec));
			});

			script->BindFunction("ve_math_abs",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Abs(dec));
			});
			script->BindFunction("ve_math_floor",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Floor(dec));
			});
			script->BindFunction("ve_math_ceil",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Ceil(dec));
			});
			script->BindFunction("ve_math_round",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				return std::make_shared<ScriptDec>(ve::dec_t::Round(dec));
			});
			script->BindFunction("ve_math_pow",
				[](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				GET_ARG_DEC_CHECKED(args, 0, dec);
				GET_ARG_DEC_CHECKED(args, 1, exp);
				return std::make_shared<ScriptDec>(ve::dec_t::Pow(dec, exp));
			});
		}
	}

	script->BindFunction("ve_make_const",
		[this](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_VAR_CHECKED(args, 0, var);
		var->_const = true;
		return var;
	});

	script->BindFunction("ve_assert",
		[this](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_BOOL_CHECKED(args, 0, expr);
		GET_ARG_STRING_CHECKED(args, 1, assertString);

		if(!expr)
		{
			throw ScriptError("Assertion Failed: " + assertString);
		}

		_debug->VE_LOG("Assertion Successful: " + assertString, LogItem::Type::ScriptLog);

		return nullptr;
	});
}

void ScriptManager::CacheGlobalVariables()
{
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_GENERIC", std::make_shared<ScriptDec>(ve::dec_t((int)CharacterStateFlagType::Generic))));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_INVULN", std::make_shared<ScriptDec>(ve::dec_t((int)CharacterStateFlagType::Invuln))));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_TARGET", std::make_shared<ScriptDec>(ve::dec_t((int)CharacterStateFlagType::CancelTargets))));
	_globalVariables.emplace(std::make_pair("VE_STATE_FLAG_CANCEL_REQUIREMENT", std::make_shared<ScriptDec>(ve::dec_t((int)CharacterStateFlagType::CancelRequirements))));
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
		GET_ARG_STRING_CHECKED(args, 0, functionName);
		ScriptArgumentCollection parentFunctionArgs = ScriptArgumentCollection(args.begin() + 1, args.end());
		character._characterScript->ExecuteFunction(functionName, parentFunctionArgs);
		return nullptr;
	});	

	script->BindFunction("character_util_getCharacterVar",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_STRING_CHECKED(args, 0, varName);
		return character._characterScript->GetVariable(varName);
	});

	script->BindFunction("character_util_setCharacterVar",
		[&character](const Script*, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
	{
		GET_ARG_STRING_CHECKED(args, 0, varName);
		GET_ARG_VAR_CHECKED(args, 1, value);
		character._characterScript->AddVariable(varName, value, true);
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
