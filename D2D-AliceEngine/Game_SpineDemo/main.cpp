#pragma once
#include "Game_SpineDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	Game_SpineDemo* pGame = new Game_SpineDemo();
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
