#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "FilesystemManager.h"
#include "CharacterStateManager.h"
#include "GamePlayer.h"
#include "GameCharacterData.h"
#include "Delegate.h"
#include "ServiceManager.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

CharacterStateManager* GameCharacter::stateManager()
{
	return _stateManager.get();
}

void GameCharacter::SetOwner(GamePlayer* owner)
{
	_playerOwner = owner;
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
	if(JSON::TryGetMember<std::string>(j, "dataPath", _dataPath))
	{
		json* dataJson = _resource->GetJsonData(_dataPath);
		_stateManager = std::make_unique<CharacterStateManager>(this, _serviceManager, j["states"], j["frames"]);

		JSON::TryGetMember<glm::vec2>(j, "sizeMultiplier", _sizeMultiplier);

		_characterScript = _scriptManager->GetScript(JSON::Get<std::string>(j["characterScript"]));
		_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
	}
}

GameCharacter::~GameCharacter()
{

}