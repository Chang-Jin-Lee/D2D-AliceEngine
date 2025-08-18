#include "EffectDemoApp.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <Scene/EffectDemoScene.h>

void EffectDemoApp::Initialize()
{
	__super::Initialize();
	SceneManager::AddScene<EffectDemoScene>(L"EffectTestScene");
	SceneManager::ChangeScene(L"EffectTestScene");
}

void EffectDemoApp::Run()
{
	__super::Run();
	MSG msg = {};
	while (!m_quit)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}
}

void EffectDemoApp::Render()
{
	__super::Render();
}

void EffectDemoApp::Update()
{
	__super::Update();
}

void EffectDemoApp::Uninitialize()
{
	__super::Uninitialize();
} 
