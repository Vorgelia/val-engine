#include "CharacterStateComponent.h"
#include "ServiceManager.h"
#include "CharacterFrame.h"
#include "CharacterState.h"
#include "CharacterSprite.h"
#include "GameCharacter.h"
#include "Script.h"
#include "ScriptVariable.h"
#include "ScriptManager.h"
#include "InputDevice.h"
#include "FilesystemManager.h"
#include "ResourceManager.h"
#include "GamePlayer.h"
#include "GameCharacterData.h"

void CharacterStateComponent::Init()
{
	GameCharacterData* characterData = _owner->_characterData.get();
	for(const std::string& path : characterData->_statePaths)
	{
		_filesystem->ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}

			const json* stateJson = _resource->GetJsonData(path.string());
			if(stateJson == nullptr)
			{
				return;
			}

			for(auto& iter : *stateJson)
			{
				auto result = this->_stateLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["name"]),
					std::make_unique<CharacterState>(iter, _scriptManager->AddScript(JSON::Get<std::string>(iter["script"])))));
				_scriptManager->HandleScriptCharacterBindings(*_owner, result.first->second->script());
			}
		});
	}

	for(const std::string& path : characterData->_frameDataPaths)
	{
		_filesystem->ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}

			const json* frameJson = _resource->GetJsonData(path.string());
			if(frameJson == nullptr)
			{
				return;
			}

			for(auto& iter : *frameJson)
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

void CharacterStateComponent::Update()
{
	if(_freezeFrameCount > 0)
	{
		_freezeFrameCount -= 1;
		return;
	}

	if(_currentState != nullptr)
	{
		_currentStateFrame += 1;

		if(_currentStateFrame == 0 && _currentState->script()->HasFunction("StateInit"))
		{
			_currentState->script()->ExecuteFunction("StateInit");
		}

		_currentState->script()->ExecuteFunction("StateUpdate",
			std::vector<std::shared_ptr<BaseScriptVariable>>{
			std::make_shared<ScriptDec>(ve::dec_t(_currentStateFrame))});
	}
}

void CharacterStateComponent::EvaluateNextState()
{
	if(_owner->_playerOwner == nullptr || _owner->_playerOwner->inputDevice() == nullptr)
	{
		return;
	}

	const CharacterState* nextState = nullptr;

	for(auto& i : _stateLookup)
	{
		if(!_allowStateSelfCancelling && !_stateEnded && i.second.get() == _currentState)
		{
			continue;
		}

		if(_owner->_playerOwner->inputDevice()->EvaluateMotion(i.second->associatedMotion()))
		{
			if(nextState == nullptr || nextState->priority() < i.second->priority())
			{
				const std::unordered_set<std::string>& stateTypeFlags = i.second->stateTypeFlags();

				const std::unordered_set<std::string>& cancelFlags = GetFlags(CharacterStateFlagType::CancelTargets);
				bool validCancel = _stateEnded || _currentState == nullptr;
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

	if(nextState != nullptr)
	{
		StartState(nextState->name());
	}
}

bool CharacterStateComponent::StartState(std::string name)
{
	auto iter = _stateLookup.find(name);
	if(iter != _stateLookup.end())
	{
		_currentState = iter->second.get();
		_currentStateId = name;
		_currentStateFrame = -1;

		_usedHitboxSequenceIDs.clear();

		_stateEnded = false;

		ClearFlags();
		Update();

		return true;
	}

	return false;
}

bool CharacterStateComponent::SetFrame(std::string name)
{
	auto iter = _frameLookup.find(name);
	if(iter != _frameLookup.end())
	{
		_currentFrame = iter->second.get();
		return true;
	}

	return false;
}

bool CharacterStateComponent::ModifyCurrentStateFrame(int newFrame)
{
	_currentStateFrame = newFrame;
	_stateEnded = false;
	return true;
}

bool CharacterStateComponent::RestartState()
{
	return StartState(_currentStateId);
}

void CharacterStateComponent::MarkStateEnded()
{
	_stateEnded = true;
}

void CharacterStateComponent::Freeze(int duration)
{
	_freezeFrameCount = duration;
}

void CharacterStateComponent::Unfreeze()
{
	_freezeFrameCount = 0;
}

bool CharacterStateComponent::AddFlag(CharacterStateFlagType type, std::string flag)
{
	auto iter = _flags.find(type);
	if(iter == _flags.end())
	{
		iter = _flags.emplace(type, std::unordered_set<std::string>()).first;
	}

	return iter->second.emplace(flag).second;
}

bool CharacterStateComponent::RemoveFlag(CharacterStateFlagType type, std::string flag)
{
	auto iter = _flags.find(type);
	if(iter == _flags.end())
	{
		return false;
	}

	return iter->second.erase(flag) != 0;
}

void CharacterStateComponent::ClearFlags()
{
	_flags.clear();
}

const std::unordered_set<std::string>& CharacterStateComponent::GetFlags(CharacterStateFlagType type)
{
	auto iter = _flags.find(type);
	if(iter == _flags.end())
	{
		iter = _flags.emplace(type, std::unordered_set<std::string>()).first;
	}

	return iter->second;
}

CharacterStateComponent::CharacterStateComponent(GameCharacter* owner, ServiceManager* serviceManager) 
	: GameCharacterComponent(owner, serviceManager)
{
	_input = serviceManager->Input();
	_scriptManager = serviceManager->ScriptManager();
	_filesystem = serviceManager->Filesystem();
	_resource = serviceManager->ResourceManager();
}