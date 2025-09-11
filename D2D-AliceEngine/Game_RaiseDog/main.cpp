#pragma once
#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	GameApp* pGame = new GameApp();
	if (pGame != nullptr)
	{
		pGame->Initialize();
		pGame->Run();
		pGame->Uninitialize();
	}
	else
	{
		assert("DemoGame instance not created in WinMain!");
	}
}
