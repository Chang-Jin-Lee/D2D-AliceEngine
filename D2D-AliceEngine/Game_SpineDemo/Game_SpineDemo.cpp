#pragma once
#include "Game_SpineDemo.h"
#include "Scene/SpineDemoScene.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>

Game_SpineDemo::Game_SpineDemo()
{
}

Game_SpineDemo::~Game_SpineDemo()
{

}

void Game_SpineDemo::Initialize()
{
	__super::Initialize();

	SceneManager::AddScene<SpineDemoScene>(L"SpineDemoScene");
	SceneManager::ChangeScene(L"SpineDemoScene");
}

void Game_SpineDemo::Run()
{
	__super::Run();
	// �޽��� ����
	MSG msg = {};
	while (!m_quit/*msg.message != WM_QUIT*/)
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

void Game_SpineDemo::Render()
{
	__super::Render();
}

void Game_SpineDemo::Update()
{
	__super::Update();
}

void Game_SpineDemo::Uninitialize()
{
	__super::Uninitialize();
}
