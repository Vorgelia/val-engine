#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceLoader.h"
#include "CharacterStateManager.h"
#include "GamePlayer.h"
#include "Delegate.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

CharacterStateManager* GameCharacter::stateManager()
{
	return _stateManager.get();
}

//TODO: I'm not particularly happy about resource management here. Figure out something better.
void GameCharacter::HandleCharacterData(const json& j)
{
	_stateManager = std::make_unique<CharacterStateManager>(this, j["states"], j["frames"]);

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

	_stateManager->EvaluateNextState();

	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}
}

void GameCharacter::GameUpdate()
{
	CharacterUpdate();

	_stateManager->StateUpdate();
}

GameCharacter::GameCharacter(Object* owner, const json& j) : Behaviour(owner, j)
{
	_initialized = false;
	_dataPath = JSON::Get<std::string>(j["dataPath"]);
	HandleCharacterData(
		ResourceLoader::LoadJsonResource(_dataPath));

	//player owner
}

GameCharacter::~GameCharacter()
{

}
