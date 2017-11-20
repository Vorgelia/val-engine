#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "JSON.h"

class CharacterState;
class CharacterFrame;

enum class CharacterStateFlagType
{
	Generic = 0,
	Invuln = 1,
	CancelTargets = 2,
	CancelRequirements = 3,
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
	std::unordered_map<CharacterStateFlagType, std::unordered_set<std::string>> _flags;

	void EvaluateNextState();

	void StateUpdate();

	bool StartState(std::string name);
	bool SetFrame(std::string name);
	bool ModifyCurrentStateFrame(int newFrame);
	bool RestartState();
	void MarkStateEnded();

	bool AddFlag(CharacterStateFlagType type, std::string flag);
	bool RemoveFlag(CharacterStateFlagType type, std::string flag);
	void ClearFlags();
	const std::unordered_set<std::string>& GetFlags(CharacterStateFlagType type);

public:
	CharacterStateManager(GameCharacter* owner, const json& states, const json& frames);
	~CharacterStateManager();
};

