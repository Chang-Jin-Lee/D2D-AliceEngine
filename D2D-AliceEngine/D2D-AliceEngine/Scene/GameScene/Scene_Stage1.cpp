#include "Scene_Stage1.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Component/AudioComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../../Scripts/BackGroundImage.h"
#include "../../Scripts/BackGroundVideo.h"
#include "../../Prefab/Truck.h"
#include "../../Scripts/BackGroundRender.h"
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include <TileMap/TileMapComponent.h>
#include <Component/BackGroundComponent.h>
#include <Component/SkewTransform.h>
#include <Effect/Prism.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/ButtonComponent.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Camera/CameraMover.h>
#include <Prefab/Player/PlayerBike.h>
#include "Scripts/TileMap/TileMapManager.h"
#include <GameManager/BulletManager.h>
#include "Scripts/Enemy/Spawn/EnemySpawner.h"
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Widget/StageWidgetScript.h>
#include <Scripts/Enemy/Spawn/SpawnCollider.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Enemy/SpawnerUsingSingleton/EnemySpawnTriggerBox.h>
#include "Scripts/Enemy/Spawn/EnemyDespawner.h"
#include <Component/Effect/ParticleComponent.h>
#include <Scripts/Audio/StageAudioScript.h>
#include <Scripts/Widget/LocationIndicatorWidgetScript.h>
#include "Scripts/Audio.h"
#include <Scripts/Legacy/CameraController.h>

void Scene_Stage1::Initialize()
{
	__super::Initialize();
}

void Scene_Stage1::Release()
{
	__super::Release();
}

void Scene_Stage1::Update()
{
    __super::Update();

	GamePlayManager::GetInstance().AddPassedTime(TimerManager::GetInstance().unscaledDeltaTime);
	if (m_player)
	{
		if (m_player->GetPosition().x >= GamePlayManager::GetInstance().GetStopXAxis() - GamePlayManager::GetInstance().GetEndSubDistance())
		{
			GamePlayManager::GetInstance().GameClear();
		}
	}
}

void Scene_Stage1::OnEnter()
{
	__super::OnEnter();
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<StageAudioScript>();

	m_mouseParticle->ToggleMouseTrail();

	//gameObject* m_camera = NewObject<gameObject>(L"Camera");
	//m_camera->AddComponent<CameraController>();
	
	// 스테이지 진입 시 마우스 상태 초기화
	Input::ResetMouseState();

	m_UI = NewObject<gameObject>(L"UI");
	//m_UI->AddComponent<TitleUIScript>();
	// 진입 시 글로벌 타임스케일 0으로 두고 컷씬 먼저 재생
	TimerManager::GetInstance().SetGlobalTimeScale(0.0f);
	// 컷씬 위젯 추가 및 다음 씬 이름 주입
	if (CutSceneWidgetScript* cut = m_UI->AddComponent<CutSceneWidgetScript>())
		cut->m_nextSceneName = m_nextSceneName;

	// 카메라컨트롤러 생성
	m_cameraController = NewObject<gameObject>(L"Camera");
	m_cameraController->AddComponent<CameraMover>();

	// 플레이어 생성
  	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<PlayerBike>();
	m_player->AddComponent<BackGroundRender>();

	BulletManager::GetInstance().SetPlayer(m_player);
	GamePlayManager::GetInstance().SetPlayer(m_player);
	GamePlayManager::GetInstance().SetPassedTime(0);
	GamePlayManager::GetInstance().SetKillEnemyAmount(0);

	// 게임 일시정지 시키기
	GamePlayManager::GetInstance().PauseGame();

	//m_bg = NewObject<gameObject>(L"BackGround");
	//m_bg->AddComponent<BackGroundVideo>()->SetPlayer(m_player);

	// 타일맵 추가
	m_tile = NewObject<gameObject>(L"TileMap");
	m_tileMapComponent = m_tile->AddComponent<TileMapComponent>();
	if (auto* tileMgr = m_tile->AddComponent<TileMapManager>())
	{
		tileMgr->SetTilePaths(
			L"TileMap/stage01_real/stage01_real.tmj",
			L"TileMap/stage01_real/Tile_Road.tsj",
			L"TileMap/TileMapColiderInfo.json");
	}

	// 적 스포너 매니저 생성
	auto tb = m_player->AddComponent<EnemySpawnTriggerBox>();
	tb->SetBox(FVector2(3300.0f, 1920.0f), 1);
	//owner->transform()->SetPivot(0.5f, 0.5f);

	// 디스포너 생성
	m_player->AddComponent<EnemyDespawner>();

	m_widget = NewObject<gameObject>(L"Widget");
	m_widget->AddComponent<StageWidgetScript>();

	// 로케이션 인디케이터 위젯
	auto hud = GetWorld()->NewObject<gameObject>(L"LocationHUD");
	hud->AddComponent<LocationIndicatorWidgetScript>();
	hud->GetComponent<LocationIndicatorWidgetScript>()->SetStartX(m_player->GetPosition().x);

	//// Truck(점프대)
	//m_truck = NewObject<gameObject>(L"Truck");
	//m_truck->AddComponent<Truck>();

	// 디버그용 씬 전환
	gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
	sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_3)) 
		{
			SceneManager::ChangeScene(L"TitleScene");
		}
		if (Input::IsKeyPressed(VK_4)) 
		{
			GamePlayManager::GetInstance().GameClear();
		}
		if (Input::IsKeyPressed(VK_5)) 
		{
			GamePlayManager::GetInstance().GameOver();
		}
		if (Input::IsKeyPressed(VK_6)) 
		{
			GamePlayManager::GetInstance().PlayBossMode();
		}
		if (Input::IsKeyDown(VK_P)) 
		{
			if (BikeMovementScript* t = m_player->GetComponent<BikeMovementScript>())
			{
				t->AddMaxSpeed(50);
			}
		}
		/*
		if (Input::IsKeyPressed(VK_O)) {
			if (BikeMovementScript* t = m_player->GetComponent<BikeMovementScript>())
			{
				t->AddMaxSpeed(-50);
			}
		}*/
	});
}

void Scene_Stage1::OnExit()
{
	__super::OnExit();
	
	// 스테이지 종료 시 마우스 상태 초기화
	Input::ResetMouseState();
	
	GamePlayManager::GetInstance().ReleaseTimers();
	BulletManager::GetInstance().SetPlayer(nullptr);
	GamePlayManager::GetInstance().SetPlayer(nullptr);
}
