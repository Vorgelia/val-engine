#pragma once
#include "GameState.h"
class GS_Intro :
	public GameState
{
public:

	float levelTimer = 0;

	void Init();
	void GUI();
	void GameUpdate();
	GS_Intro(FS::path path);
};

