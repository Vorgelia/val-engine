#pragma once
#include "GameScene.h"

class GS_Menu :
	public GameScene
{
public:
	void FrameEnd();
	void RenderUI();
	void Update();
	void GameUpdate();
	void Init();

	GS_Menu(const FS::path& path);
};
