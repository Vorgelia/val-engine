#include "CharacterStateManager.h"
#include "CharacterFrame.h"
#include "CharacterState.h"
#include "GameCharacter.h"
#include "Script.h"
#include "ScriptVariable.h"
#include "ScriptManager.h"
#include "InputManager.h"
#include "InputDevice.h"
#include "InputMotion.h"
#include "ResourceLoader.h"

void CharacterStateManager::StateUpdate()
{
	if(_currentState != nullptr)
	{
		_currentStateFrame += 1;

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
		//TODO: Evaluate motion on input device of owner
		if(InputManager::_inputDevices[-1]->EvaluateMotion(i.second->associatedMotion()))
		{
			if(nextState == nullptr || nextState->priority() < i.second->priority())
			{
				nextState = i.second.get();
			}
		}
	}

	//TODO: Add cancelling rules
	if(_stateEnded)
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

CharacterStateManager::CharacterStateManager(GameCharacter* owner, const json& states, const json& frames) :
	_owner(owner)
{
	for(auto& iter : states)
	{
		std::string path = JSON::Get<std::string>(iter);
		ResourceLoader::ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}
			const json& stateJson = ResourceLoader::LoadJsonResource(path);
			for(auto& iter : stateJson)
			{
				auto& result = this->_stateLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["name"]),
					std::make_unique<CharacterState>(iter)));
				ScriptManager::HandleScriptCharacterBindings(*_owner, result.first->second->script());
			}
		});
	}

	for(auto& iter : frames)
	{
		std::string path = JSON::Get<std::string>(iter);
		ResourceLoader::ApplyFunctionToFiles(path, [this](const FS::path path)
		{
			if(path.extension().string() != ".json")
			{
				return;
			}
			const json& frameJson = ResourceLoader::LoadJsonResource(path);
			for(auto& iter : frameJson)
			{
				this->_frameLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["id"]),
					std::make_unique<CharacterFrame>(iter)));
			}
		});
	}
}

CharacterStateManager::~CharacterStateManager()
{
}
