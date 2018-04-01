#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "CharacterStateComponent.h"
#include "CharacterPhysicsComponent.h"
#include "GamePlayer.h"
#include "CharacterState.h"
#include "CharacterFrame.h"
#include "GameCharacterData.h"
#include "ServiceManager.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

const GameCharacterData* GameCharacter::characterData() const
{
	return _characterData.get();
}

CharacterStateComponent* GameCharacter::stateComponent() const
{
	return _stateComponent.get();
}

CharacterPhysicsComponent* GameCharacter::physicsComponent() const
{
	return _physicsComponent.get();
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

	_stateComponent->EvaluateNextState();

	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}
}

void GameCharacter::GameUpdate()
{
	CharacterUpdate();

	_stateComponent->StateUpdate();
	_physicsComponent->Update();
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
			_stateComponent = std::make_unique<CharacterStateComponent>(this, _serviceManager);
			_physicsComponent = std::make_unique<CharacterPhysicsComponent>(this, serviceManager);

			std::string scriptPath;
			if(JSON::TryGetMember(j, "characterScript", scriptPath))
			{
				_characterScript = _scriptManager->GetScript(scriptPath);
				_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
			}
		}
	}
}