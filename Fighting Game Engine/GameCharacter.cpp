#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ScriptVariable.h"
#include "CharacterFrame.h"
#include "CharacterState.h"
#include "ResourceLoader.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

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
				this->_stateLookup.insert(std::make_pair(
					JSON::Get<std::string>(iter["name"]),
					std::make_unique<CharacterState>(iter)));
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
}

void GameCharacter::GameUpdate()
{
	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}

	/*
	_currentStateFrame += 1;
	_currentState->script()->ExecuteFunction("StateUpdate",
		std::vector<std::shared_ptr<BaseScriptVariable>>{
		std::make_shared<ScriptInt>(_currentStateFrame)});*/

	//TODO: Update
	_currentState = _stateLookup.begin()->second.get();
	_currentFrame = _frameLookup.begin()->second.get();
}

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

GameCharacter::GameCharacter(Object* owner, const json& j) : Behaviour(owner, j)
{
	_flipped = JSON::Get<bool>(j["flipped"]);
	_dataPath = JSON::Get<std::string>(j["dataPath"]);
	HandleCharacterData(
		ResourceLoader::LoadJsonResource(_dataPath));
}

GameCharacter::~GameCharacter()
{

}
