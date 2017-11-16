#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ScriptVariable.h"
#include "CharacterFrame.h"
#include "CharacterState.h"
#include "ResourceLoader.h"
#include "DebugLog.h"
#include "Time.h"
#include "InputManager.h"
#include "InputDevice.h"
#include "InputMotion.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

const CharacterFrame* GameCharacter::currentFrame()
{
	return _currentFrame;
}

glm::vec2 GameCharacter::sizeMultiplier()
{
	return _sizeMultiplier;
}

bool GameCharacter::flipped()
{
	return _flipped;
}

//TODO: I'm not particularly happy about resource management here. Figure out something better.
void GameCharacter::HandleCharacterData(const json& j)
{
	for(auto& iter : j["states"])
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
				ScriptManager::HandleScriptCharacterBindings(*this, result.first->second->script());
			}
		});
	}

	for(auto& iter : j["frames"])
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

	_sizeMultiplier = JSON::Get<glm::vec2>(j["sizeMultiplier"]);

	_characterScript = ScriptManager::GetScript(JSON::Get<std::string>(j["characterScript"]));
	ScriptManager::HandleScriptCharacterBindings(*this, _characterScript);
}

void GameCharacter::CharacterInit()
{
	if(_characterScript != nullptr && _characterScript->HasFunction("CharacterInit"))
	{
		_characterScript->ExecuteFunction("CharacterInit");
	}
	_initialized = true;
}

void GameCharacter::CharacterUpdate()
{
	if(!_initialized)
	{
		CharacterInit();
	}

	EvaluateNextState();

	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}
}

void GameCharacter::StateUpdate()
{
	if(_currentState != nullptr)
	{
		_currentStateFrame += 1;

		_currentState->script()->ExecuteFunction("StateUpdate",
			std::vector<std::shared_ptr<BaseScriptVariable>>{
			std::make_shared<ScriptInt>(_currentStateFrame)});
	}
}

void GameCharacter::EvaluateNextState()
{
	const CharacterState* nextState = nullptr;

	for(auto& i : _stateLookup)
	{
		if(InputManager::_inputDevices[-1]->EvaluateMotion(std::vector<InputMotionComponent>(i.second->associatedMotion()), _flipped))
		{
			if(nextState == nullptr || nextState->priority() < i.second->priority())
			{
				nextState = i.second.get();
			}
		}
	}

	if(nextState != _currentState)
	{
		StartState(nextState->name());
	}
	else if(_stateEnded)
	{
		RestartState();
	}
}

void GameCharacter::GameUpdate()
{
	CharacterUpdate();

	StateUpdate();
}

bool GameCharacter::StartState(std::string name)
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

bool GameCharacter::SetFrame(std::string name)
{
	auto& iter = _frameLookup.find(name);
	if(iter != _frameLookup.end())
	{
		_currentFrame = iter->second.get();
		return true;
	}

	return false;
}

bool GameCharacter::ModifyCurrentStateFrame(int newFrame)
{
	_currentStateFrame = newFrame;
	_stateEnded = false;
	return true;
}

bool GameCharacter::RestartState()
{
	return StartState(_currentStateId);
}

void GameCharacter::MarkStateEnded()
{
	_stateEnded = true;
}

GameCharacter::GameCharacter(Object* owner, const json& j) : Behaviour(owner, j)
{
	_initialized = false;
	_dataPath = JSON::Get<std::string>(j["dataPath"]);
	HandleCharacterData(
		ResourceLoader::LoadJsonResource(_dataPath));
}

GameCharacter::~GameCharacter()
{

}
