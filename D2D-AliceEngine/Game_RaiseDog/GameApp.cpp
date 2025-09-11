#pragma once
#include "GameApp.h"
#include "Scene/MainMenuScene.h"
#include "Scene/HomeScene.h"
#include "Scene/ParkScene.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <Application.h>

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

void GameApp::Initialize()
{
	__super::Initialize();

	// 게임별 해상도 적용 (세로 화면)
	//SetResolution(1280, 720);

	SceneManager::AddScene<MainMenuScene>(L"MainMenu");
	SceneManager::AddScene<HomeScene>(L"Home");
	SceneManager::AddScene<ParkScene>(L"Park");
	SceneManager::ChangeScene(L"MainMenu");
}

void GameApp::Run()
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

void GameApp::Render()
{
	__super::Render();
}

void GameApp::Update()
{
	__super::Update();
}

void GameApp::Uninitialize()
{
	__super::Uninitialize();
} 
