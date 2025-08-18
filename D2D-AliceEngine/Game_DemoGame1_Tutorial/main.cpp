#pragma once
#include "Game_DemoGame1_Tutorial.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	Game_DemoGame1_Tutorial* pGame = new Game_DemoGame1_Tutorial();
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
