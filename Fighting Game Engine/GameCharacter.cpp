#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceLoader.h"
#include "CharacterStateManager.h"
#include "GamePlayer.h"
#include "Delegate.h"
#include "ServiceManager.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

CharacterStateManager* GameCharacter::stateManager()
{
	return _stateManager.get();
}

//TODO: I'm not particularly happy about resource management here. Figure out something better.
void GameCharacter::HandleCharacterData(const json& j)
{
	_stateManager = std::make_unique<CharacterStateManager>(this, _serviceManager, j["states"], j["frames"]);

	_sizeMultiplier = JSON::Get<glm::vec2>(j["sizeMultiplier"]);

	_characterScript = _scriptManager->GetScript(JSON::Get<std::string>(j["characterScript"]));
	_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
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

GameCharacter::GameCharacter(Object* owner, ServiceManager* serviceManager, const json& j) : Behaviour(owner, serviceManager, j)
{
	_filesystem = serviceManager->Filesystem();
	_scriptManager = serviceManager->ScriptManager();
	_resource = serviceManager->ResourceManager();

	_initialized = false;
	_dataPath = JSON::Get<std::string>(j["dataPath"]);
	HandleCharacterData(_filesystem->LoadJsonResource(_dataPath));

	//TODO: player owner
}

GameCharacter::~GameCharacter()
{

}
