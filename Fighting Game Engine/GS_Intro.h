#pragma once
#include "GameScene.h"

class GS_Intro :
	public GameScene
{
	float _levelTimer = 0;
public:

	void Init();
	void RenderUI();
	void GameUpdate();

	GS_Intro(const FS::path& path);
};