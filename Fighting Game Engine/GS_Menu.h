#pragma once
#include "GameState.h"

class GS_Menu :
	public GameState
{
public:
	void FrameEnd();
	void GUI();
	void Update();
	void GameUpdate();
	void Init();

	GS_Menu(const FS::path& path);
};

