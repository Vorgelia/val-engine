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

const GameCharacterData * GameCharacter::characterData() const
{
	return _characterData.get();
}

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

	std::string dataPath;
	if(JSON::TryGetMember<std::string>(j, "dataPath", dataPath))
	{
		json* dataJson = _resource->GetJsonData(dataPath);
		if(dataJson != nullptr)
		{
			_characterData = std::make_unique<GameCharacterData>(*dataJson);
			_stateManager = std::make_unique<CharacterStateManager>(this, _serviceManager);

			std::string scriptPath;
			if(JSON::TryGetMember(j, "characterScript", scriptPath))
			{
				_characterScript = _scriptManager->GetScript(scriptPath);
				_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
			}
		}
	}
}

GameCharacter::~GameCharacter()
{

}