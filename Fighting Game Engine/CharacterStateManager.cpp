#include "CharacterStateManager.h"
#include "ServiceManager.h"
#include "CharacterFrame.h"
#include "CharacterState.h"
#include "CharacterSprite.h"
#include "GameCharacter.h"
#include "Script.h"
#include "ScriptVariable.h"
#include "ScriptManager.h"
#include "InputManager.h"
#include "InputDevice.h"
#include "InputMotion.h"
#include "FilesystemManager.h"
#include "ResourceManager.h"

void CharacterStateManager::StateUpdate()
{
	if(_currentState != nullptr)
	{
		_currentStateFrame += 1;

		if(_currentStateFrame == 0 && _currentState->script()->HasFunction("StateInit"))
		{
			_currentState->script()->ExecuteFunction("StateInit");
		}

		_currentState->script()->ExecuteFunction("StateUpdate",
			std::vector<std::shared_ptr<BaseScriptVariable>>{
			std::make_shared<ScriptInt>(_currentStateFrame)});
	}
}

void CharacterStateManager::EvaluateNextState()
{
	const CharacterState* nextState = nullptr;

	for(auto& i : _stateLookup)
	{
		if(!_allowStateSelfCancelling && !_stateEnded && i.second.get() == _currentState)
		{
			continue;
		}

		//TODO: Evaluate motion on input device of owner
		if(_input->GetInputDevice(-1)->EvaluateMotion(i.second->associatedMotion()))
		{
			if(nextState == nullptr || nextState->priority() < i.second->priority())
			{
				const std::unordered_set<std::string>& stateTypeFlags = i.second->stateTypeFlags();

				const std::unordered_set<std::string>& cancelFlags = GetFlags(CharacterStateFlagType::CancelTargets);
				bool validCancel = _stateEnded;
				for(auto& iter : stateTypeFlags)
				{
					if(cancelFlags.find(iter) != cancelFlags.end())
					{
						validCancel = true;
						break;
					}
				}

				if(validCancel)
				{
					const std::unordered_set<std::string>& cancelRequirements = GetFlags(CharacterStateFlagType::CancelRequirements);
					for(auto& iter : cancelRequirements)
					{
						if(stateTypeFlags.find(iter) == stateTypeFlags.end())
						{
							validCancel = false;
							break;
						}
					}
				}

				if(validCancel)
				{
					nextState = i.second.get();
				}
			}
		}
	}

	//TODO: Add cancelling rules
	if(nextState != nullptr)
	{
		StartState(nextState->name());
	}
}

bool CharacterStateManager::StartState(std::string name)
{
	auto& iter = _stateLookup.find(name);
	if(iter != _stateLookup.end())
	{
		_currentState = iter->second.get();
		_currentStateId = name;
		_currentStateFrame = -1;

		_stateEnded = false;

		ClearFlags();
		StateUpdate();

		return true;
	}

	return false;
}

bool CharacterStateManager::SetFrame(std::string name)
{
	auto& iter = _frameLookup.find(name);
	if(iter != _frameLookup.end())
	{
		_currentFrame = iter->second.get();
		return true;
	}

	return false;
}

bool CharacterStateManager::ModifyCurrentStateFrame(int newFrame)
{
	_currentStateFrame = newFrame;
	_stateEnded = false;
	return true;
}

bool CharacterStateManager::RestartState()
{
	return StartState(_currentStateId);
}

void CharacterStateManager::MarkStateEnded()
{
	_stateEnded = true;
}

bool CharacterStateManager::AddFlag(CharacterStateFlagType type, std::string flag)
{
	auto& iter = _flags.find(type);
	if(iter == _flags.end())
	{
		iter = _flags.emplace(type, std::unordered_set<std::string>()).first;
	}

	return iter->second.emplace(flag).second;
}

bool CharacterStateManager::RemoveFlag(CharacterStateFlagType type, std::string flag)
{
	auto& iter = _flags.find(type);
	if(iter == _flags.end())
	{
		return false;
	}

	return iter->second.erase(flag) != 0;
}

void CharacterStateManager::ClearFlags()
{
	_flags.clear();
}

const std::unordered_set<std::string>& CharacterStateManager::GetFlags(CharacterStateFlagType type)
{
	auto& iter = _flags.find(type);
	if(iter == _flags.end())
	{
		iter = _flags.emplace(type, std::unordered_set<std::string>()).first;
	}

	return iter->second;
}

CharacterStateManager::CharacterStateManager(GameCharacter* owner, ServiceManager* serviceManager, const json& states, const json& frames) :
	_owner(owner)
{
	_input = serviceManager->Input();
	_scriptManager = serviceManager->ScriptManager();
	_filesystem = serviceManager->Filesystem();

	for(auto& iter : states)
	{
		std::string path = JSON::Get<std::string>(iter);
		_filesystem->ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}
			const json& stateJson = _filesystem->LoadJsonResource(path);
			for(auto& iter : stateJson)
			{
				auto& result = this->_stateLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["name"]),
					std::make_unique<CharacterState>(iter,  _scriptManager->AddScript(JSON::Get<std::string>(iter["script"])))));
				_scriptManager->HandleScriptCharacterBindings(*_owner, result.first->second->script());
			}
		});
	}

	for(auto& iter : frames)
	{
		std::string path = JSON::Get<std::string>(iter);
		_filesystem->ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}
			const json& frameJson = _filesystem->LoadJsonResource(path);
			for(auto& iter : frameJson)
			{
				this->_frameLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["id"]),
					std::make_unique<CharacterFrame>(iter)));
			}
		});
	}

	for(auto& iter : _frameLookup)
	{
		_owner->_resource->GetTexture(iter.second->spriteData()->sprite());
	}
}

CharacterStateManager::~CharacterStateManager()
{
}
