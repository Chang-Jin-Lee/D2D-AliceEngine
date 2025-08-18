#pragma once
#include "EffectDemoApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	EffectDemoApp* pGame = new EffectDemoApp();
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
