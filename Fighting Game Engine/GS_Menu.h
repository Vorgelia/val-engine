#pragma once
#include "GameScene.h"

class GS_Menu :
	public GameScene
{
public:
	void RenderUI();
	void Update();
	void GameUpdate();

	GS_Menu(const FS::path& path);
};
