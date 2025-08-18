#pragma once
#include "Game_DemoGame1_Tutorial.h"
#include "Scene/DemoScene1_Tutorial.h"
#include "Scene/DemoScene2_Tutorial.h"
#include "Scene/DemoScene3_Tutorial.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>

Game_DemoGame1_Tutorial::Game_DemoGame1_Tutorial()
{
}

Game_DemoGame1_Tutorial::~Game_DemoGame1_Tutorial()
{

}

void Game_DemoGame1_Tutorial::Initialize()
{
	__super::Initialize();

	SceneManager::AddScene<DemoScene1_Tutorial>(L"SolarSystemScene");
	SceneManager::AddScene<DemoScene2_Tutorial>(L"aruScene");
	SceneManager::AddScene<DemoScene3_Tutorial>(L"FSMScene");
	SceneManager::ChangeScene(L"SolarSystemScene");
}

void Game_DemoGame1_Tutorial::Run()
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

void Game_DemoGame1_Tutorial::Render()
{
	__super::Render();
}

void Game_DemoGame1_Tutorial::Update()
{
	__super::Update();
}

void Game_DemoGame1_Tutorial::Uninitialize()
{
	__super::Uninitialize();
}
