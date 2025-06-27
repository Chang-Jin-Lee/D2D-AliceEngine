#pragma once
#include "stdafx.h"
#include "DemoGameApp.h"
#include "Scene/DemoScene.h"
#include "Scene/DemoScene2.h"

DemoGameApp::DemoGameApp()
{

}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();

	SceneManager::AddScene<DemoScene2>(L"aruScene");
	DemoScene* SolarSystemScene = SceneManager::AddScene<DemoScene>(L"SolarSystemScene");
	SceneManager::ChangeScene(L"aruScene");
}

void DemoGameApp::Run()
{
	__super::Run();
	// �޽��� ����
	MSG msg = {};
	while (msg.message != WM_QUIT)
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

void DemoGameApp::Render()
{
	__super::Render();
}

void DemoGameApp::Update()
{
	__super::Update();
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();
}