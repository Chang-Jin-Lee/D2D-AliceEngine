#include "Scene_Stage3.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Object/gameObject.h>
#include <Component/TileMapRenderer.h>
#include <TileMap/TileMapComponent.h>
#include <Scripts/TileMap/TileMapManager.h>

#include <Manager/TimerManager.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Camera/CameraMover.h>
#include <Prefab/Player/PlayerBike.h>
#include <Scripts/BackGroundRender.h>
#include <Scripts/Enemy/Spawn/EnemySpawner.h>
#include <Prefab/Truck.h>
#include <GameManager/BulletManager.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Enemy/SpawnerUsingSingleton/EnemySpawnTriggerBox.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Widget/StageWidgetScript.h>
#include <Scripts/Enemy/Spawn/EnemyDespawner.h>
#include <Component/Effect/ParticleComponent.h>
#include <Scripts/Audio/StageAudioScript.h>
#include <Scripts/Widget/LocationIndicatorWidgetScript.h>

void Scene_Stage3::Initialize()
{
    __super::Initialize();

    m_sound = NewObject<gameObject>(L"Sound");
    m_sound->AddComponent<StageAudioScript>();
}

void Scene_Stage3::Release()
{
    __super::Release();
}

void Scene_Stage3::Update()
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

void Scene_Stage3::OnEnter()
{
    __super::OnEnter();
    m_mouseParticle->ToggleMouseTrail();
    m_textGO = NewObject<gameObject>(L"Stage3Label");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();
    text->SetText(L"<현재 씬> Scene_Stage3");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(24.0f);

    // 타일맵 추가
    m_tile = NewObject<gameObject>(L"TileMap");
    m_tile->AddComponent<TileMapComponent>();
    if (auto* tileMgr = m_tile->AddComponent<TileMapManager>())
    {
        tileMgr->SetTilePaths(
            L"TileMap/stage03_real/stage03_real.tmj",
            L"TileMap/stage03_real/Tile_Road.tsj",
            L"TileMap/TileMapColiderInfo.json");
    }

    // VK_3 누르면 TitleScene으로 전환
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(L"TitleScene");
        }
    });


    //=================================== 게임 로직 
    m_UI = NewObject<gameObject>(L"UI");
    //m_UI->AddComponent<TitleUIScript>();
    // 진입 시 글로벌 타임스케일 0으로 두고 컷씬 먼저 재생
    TimerManager::GetInstance().SetGlobalTimeScale(0.0f);
    // 컷씬 위젯 추가 및 다음 씬 이름 주입
    if (CutSceneWidgetScript* cut = m_UI->AddComponent<CutSceneWidgetScript>())
        cut->m_nextSceneName = m_nextSceneName;

    m_cameraController = NewObject<gameObject>(L"Camera");
    m_cameraController->AddComponent<CameraMover>();

    m_player = NewObject<gameObject>(L"Player");
    m_player->AddComponent<PlayerBike>();
    BulletManager::GetInstance().SetPlayer(m_player);
    GamePlayManager::GetInstance().SetPlayer(m_player);
    GamePlayManager::GetInstance().SetPassedTime(0);
    GamePlayManager::GetInstance().SetKillEnemyAmount(0);

	// 게임 일시정지 시키기
	GamePlayManager::GetInstance().PauseGame();

    m_player->AddComponent<BackGroundRender>();

    //m_bg = NewObject<gameObject>(L"BackGround");
    //m_bg->AddComponent<BackGroundVideo>()->SetPlayer(m_player);

    // 적 스포너 매니저 생성
    enemySpawnTriggerBox = NewObject<gameObject>(L"EnemySpawnTriggerBox");
    auto tb = enemySpawnTriggerBox->AddComponent<EnemySpawnTriggerBox>();
    tb->SetBox(FVector2(3300.0f, 1920.0f), 1);

	// 디스포너 생성
	gameObject* deSpawner = NewObject<gameObject>(L"DeSpwaner");
	deSpawner->AddComponent<EnemyDespawner>();

	m_widget = NewObject<gameObject>(L"Widget");
	m_widget->AddComponent<StageWidgetScript>();

    // 로케이션 인디케이터 위젯
    auto hud = GetWorld()->NewObject<gameObject>(L"LocationHUD");
    auto* indicatorWidget = hud->AddComponent<LocationIndicatorWidgetScript>();
    indicatorWidget->SetStartX(m_player->GetPosition().x);
    indicatorWidget->SetBossState(true);

    // 디버그용 씬 전환
    gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
    sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
        if (Input::IsKeyPressed(VK_3)) {
            SceneManager::ChangeScene(L"TitleScene");
        }
        if (Input::IsKeyPressed(VK_4)) {
            GamePlayManager::GetInstance().GameClear();
        }
        if (Input::IsKeyPressed(VK_5)) {
            GamePlayManager::GetInstance().GameOver();
        }
        if (Input::IsKeyPressed(VK_6)) {
            GamePlayManager::GetInstance().PlayBossMode();
        }
		/*if (Input::IsKeyPressed(VK_P)) {
			if (BikeMovementScript* t = m_player->GetComponent<BikeMovementScript>())
			{
				t->AddMaxSpeed(50);
			}
		}
		if (Input::IsKeyPressed(VK_O)) {
			if (BikeMovementScript* t = m_player->GetComponent<BikeMovementScript>())
			{
				t->AddMaxSpeed(-50);
			}
		}*/
    });
}

void Scene_Stage3::OnExit()
{
    __super::OnExit();
    GamePlayManager::GetInstance().ReleaseTimers();
    BulletManager::GetInstance().SetPlayer(nullptr);
    GamePlayManager::GetInstance().SetPlayer(nullptr);
}


