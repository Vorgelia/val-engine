#pragma once
#include "GameState.h"
class GS_Intro :
	public GameState
{
	float _levelTimer = 0;
public:

	void Init();
	void GUI();
	void GameUpdate();

	GS_Intro(const FS::path& path);
};