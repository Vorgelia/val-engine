#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "JSON.h"
#include "BaseGameCharacterComponent.h"

class CharacterState;
class CharacterFrame;
class GameInstance;
class InputManager;
class FilesystemManager;
class ResourceManager;

enum class CharacterStateFlagType
{
	Generic = 0,
	Invuln = 1,
	CancelTargets = 2,
	CancelRequirements = 3,
};

class CharacterStateComponent : public BaseGameCharacterComponent
{
	friend class GameCharacter;
	friend class ScriptManager;
	friend class CharacterRenderer;
	friend class CharacterEventComponent;

private:
	InputManager* _input;
	ScriptManager* _scriptManager;
	FilesystemManager* _filesystem;
	ResourceManager* _resource;

private:
	std::string _currentStateId;
	CharacterState* _currentState;
	CharacterFrame* _currentFrame;
	int _currentStateFrame;

	int _freezeFrameCount;

	bool _allowStateSelfCancelling;
	bool _stateEnded;

	std::unordered_map<std::string, std::unique_ptr<CharacterState>> _stateLookup;
	std::unordered_map<std::string, std::unique_ptr<CharacterFrame>> _frameLookup;

	std::unordered_map<CharacterStateFlagType, std::unordered_set<std::string>> _flags;
	std::unordered_set<int> _usedHitboxSequenceIDs;

	void EvaluateNextState();

	bool StartState(const std::string& name);
	bool SetFrame(const std::string& name);
	bool ModifyCurrentStateFrame(int newFrame);
	bool RestartState();
	void MarkStateEnded();

	void Freeze(int duration);
	void Unfreeze();

	bool AddFlag(CharacterStateFlagType type, const std::string& flag);
	bool RemoveFlag(CharacterStateFlagType type, const std::string& flag);
	void ClearFlags();
	const std::unordered_set<std::string>& GetFlags(CharacterStateFlagType type);

protected:
	void OnInit() override;
	void Update();

	void CacheCharacterData();

public:
	const CharacterState* currentState() const { return _currentState; }
	const CharacterFrame* currentFrame() const { return _currentFrame; }

	CharacterStateComponent() = default;
	~CharacterStateComponent() = default;
};

