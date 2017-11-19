#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "JSON.h"

class CharacterState;
class CharacterFrame;

enum class CharacterStateFlagType
{
	Generic = 0,
	Invuln = 1,
};

class CharacterStateManager
{
	friend class GameCharacter;
	friend class ScriptManager;
	friend class CharacterRenderer;

	GameCharacter* _owner;

	std::string _currentStateId;
	CharacterState* _currentState;
	CharacterFrame* _currentFrame;
	int _currentStateFrame;
	bool _stateEnded;

	std::unordered_map<std::string, std::unique_ptr<CharacterState>> _stateLookup;
	std::unordered_map<std::string, std::unique_ptr<CharacterFrame>> _frameLookup;

	//Throw invuln, air invuln, etc
	std::unordered_map<CharacterStateFlagType, std::vector<std::string>> _flags;

	void EvaluateNextState();

	void StateUpdate();

	bool StartState(std::string name);
	bool SetFrame(std::string name);
	bool ModifyCurrentStateFrame(int newFrame);
	bool RestartState();
	void MarkStateEnded();

public:

	CharacterStateManager(GameCharacter* owner, const json& states, const json& frames);
	~CharacterStateManager();
};

