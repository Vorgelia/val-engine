#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ScriptVariable.h"
#include "CharacterFrame.h"
#include "CharacterState.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

void GameCharacter::GameUpdate()
{
	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}

	_currentStateFrame += 1;
	_currentState->script()->ExecuteFunction("StateUpdate",
		std::vector<std::shared_ptr<BaseScriptVariable>>{
		std::make_shared<ScriptInt>(_currentStateFrame)});
}

const CharacterFrame * GameCharacter::currentFrame()
{
	return _currentFrame;
}

glm::vec2 GameCharacter::sizeMultiplier()
{
	return _sizeMultiplier;
}

GameCharacter::GameCharacter(Object* owner, const json& j) : Behaviour(owner, j)
{
	for(auto& iter : j["states"])
	{
		std::string path = JSON::Get<std::string>(iter);
		//TODO:
		//if file
		//	load
		//if folder
		//	load everything in folder
	}

	for(auto& iter : j["frames"])
	{
		std::string path = JSON::Get<std::string>(iter);
		//TODO:
		//if file
		//	load
		//if folder
		//	load everything in folder
	}

	_sizeMultiplier = JSON::Get<glm::vec2>(j["sizeMultiplier"]);
	_characterScript = ScriptManager::GetScript(JSON::Get<std::string>(j["characterScript"]));
}

GameCharacter::~GameCharacter()
{

}
