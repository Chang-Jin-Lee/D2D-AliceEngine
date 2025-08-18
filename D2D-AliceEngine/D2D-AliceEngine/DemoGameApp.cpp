#pragma once
#include "DemoGameApp.h"
#include "Scene/DemoScene.h"
#include "Scene/DemoScene2.h"
#include "Scene/DemoScene3.h"
#include "Scene/DemoScene4.h"
#include "Scene/SpineScene.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>

/*-----���� �ΰ��Ӿ��� �߰��մϴ�-----*/
#include <Scene/GameScene/TitleScene.h>
#include <Scene/GameScene/Scene_Stage1.h>
#include <Scene/GameScene/Scene_Stage2.h>
#include <Scene/GameScene/Scene_Stage3.h>
#include <Scene/GameScene/GameClearScene.h>
#include <Scene/GameScene/GameOverScene.h>
#include <Scene/SelectScene.h>
#include <Scene/KangScene/KangScene.h>
#include <GameManager/BulletManager.h>
#include <GameManager/GamePlayManager.h>
#include <Scene/EffectTestScene.h>
#include <GameManager/EnemyDataManager.h>
#include <GameManager/PlayerDataManager.h>

DemoGameApp::DemoGameApp()
{
	BulletManager::Create();
}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();

	GamePlayManager::Create();
	EnemyDataManager::Create();
	EnemyDataManager::GetInstance().LoadData(L"AbilityData/EnemyData.json");
 	EnemyDataManager::GetInstance().LoadSpawnData(L"TileMap/TileMapColiderInfo.json");
	PlayerDataManager::Create();
	PlayerDataManager::GetInstance().LoadData(L"AbilityData/PlayerData.json");

	SceneManager::AddScene<DemoScene2>(L"aruScene");
	SceneManager::AddScene<DemoScene3>(L"FSMScene");
	SceneManager::AddScene<DemoScene>(L"SolarSystemScene");
	SceneManager::AddScene<DemoScene4>(L"HiroScene");
	SceneManager::AddScene<EffectTestScene>(L"EffectTestScene");
	SceneManager::AddScene<TitleScene>(L"TitleScene");		// title
	SceneManager::AddScene<SpineScene>(L"SpineScene");
	SceneManager::AddScene<KangScene>(L"KangTest");
	SceneManager::AddScene<SelectScene>(L"SelectScene");	// stage select
	SceneManager::AddScene<Scene_Stage1>(Define::Scene_Stage1);	// stage select
	SceneManager::AddScene<Scene_Stage2>(Define::Scene_Stage2);	// stage select
	SceneManager::AddScene<Scene_Stage3>(Define::Scene_Stage3);	// stage select
	SceneManager::AddScene<GameClearScene>(Define::Scene_GameClear);	// stage select
	SceneManager::AddScene<GameOverScene>(Define::Scene_GameOver);	// stage select
	SceneManager::ChangeScene(L"TitleScene");
}

void DemoGameApp::Run()
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
