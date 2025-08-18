#include "pch.h"
#include "GamePlayManager.h"
#include <Manager/SceneManager.h>  // 실제 씬 변경은 UI/외부가 하고, 여기선 이름만 관리
#include <Helpers/Logger.h>
#include <utility>
#include <Scripts/Widget/BlackOutWidgetScript.h>
#include <Scripts/Widget/VignetteWidgetScript.h>
#include <Scripts/Enemy/SpawnerUsingSingleton/EnemySpawnTriggerBox.h>
#include <Scene/Scene.h>
#include <Scripts/Player/PlayerManager.h>
#include "Scripts/Audio/StageAudioScript.h"
#include "Component/AudioComponent.h"

GamePlayManager::GamePlayManager()
{
    m_State = EGameRunState::Boot;
    m_CurrentScene.clear();
    m_PendingScene.clear();

    // 초기 타임스케일 보정
    m_PrevTimeScale = TimerManager::GetInstance().GetGlobalTimeScale();
    if (m_PrevTimeScale <= 0.0f) TimerManager::GetInstance().SetGlobalTimeScale(1.0f);
}

GamePlayManager::~GamePlayManager()
{

}

// ===== 내부 공통 =====
void GamePlayManager::SetState(EGameRunState newState)
{
    if (m_State == newState) return;
    auto prev = m_State;
    m_State = newState;
    if (m_OnStateChanged) m_OnStateChanged(prev, m_State);

    // 디버그 로그
    switch (m_State) {
    case EGameRunState::Boot:          LOG_INFO(L"[GameState] Boot"); break;
    case EGameRunState::Loading:       LOG_INFO(L"[GameState] Loading"); break;
    case EGameRunState::InGame:        LOG_INFO(L"[GameState] InGame"); break;
    case EGameRunState::Paused:        LOG_INFO(L"[GameState] Paused"); break;
    case EGameRunState::GameOver:      LOG_INFO(L"[GameState] GameOver"); break;
    case EGameRunState::Victory:       LOG_INFO(L"[GameState] Victory"); break;
    case EGameRunState::Transitioning: LOG_INFO(L"[GameState] Transitioning"); break;
    default: break;
    }
}

void GamePlayManager::ApplyPauseTimescale(bool paused)
{
    auto& TM = TimerManager::GetInstance();
    if (paused) {
        // 저장 후 0으로
        m_PrevTimeScale = TM.GetGlobalTimeScale();
        if (m_PrevTimeScale <= 0.0f) m_PrevTimeScale = 1.0f;
        TM.SetGlobalTimeScale(0.0f);
    }
    else {
        TM.SetGlobalTimeScale((m_PrevTimeScale > 0.0f) ? m_PrevTimeScale : 1.0f);
    }
}

// ===== 외부 API =====
void GamePlayManager::StartGame()
{
    // 게임을 시작하면 InGame으로 전환하고 타임스케일 복구
    GamePlayManager::GetInstance().SetPassedTime(0);
    ApplyPauseTimescale(false);
    SetState(EGameRunState::InGame);
}

void GamePlayManager::GameOver()
{
	// 사운드 멈춤
    if(StageAudioScript::instance->m_MotorSound->IsPlaying())
	    StageAudioScript::instance->m_MotorSound->StopByName(L"MotorSound");

    SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::VignetteOnly), true, 2.0f, 1.0f);

    TimerManager::GetInstance().ClearTimer(gameOverTransitionTimer);
    TimerManager::GetInstance().SetTimer(gameOverTransitionTimer, [this]()
    {
        SceneManager::ChangeScene(Define::Scene_GameOver);
    },
        0,
        false,
        3.0f);

    TimerManager::GetInstance().ClearTimer(gameOverTimer);
    TimerManager::GetInstance().SetTimer(gameOverTimer, 
        [this]() 
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::SpotDiffuse), true, 2.0f, 1.0f);
    }, 
    0, 
    false,
    1.0f);

   
}

void GamePlayManager::GameClear()
{
	// 사운드 멈춤
	if (StageAudioScript::instance->m_MotorSound->IsPlaying())
		StageAudioScript::instance->m_MotorSound->StopByName(L"MotorSound");

    SpawnVignette(2.0f, 1.0f);
    PlayerManager::instance->SetMove(false);
    //TimerManager::GetInstance().ClearTimer(gameOverTransitionTimer);
    TimerManager::GetInstance().SetTimer(gameOverTransitionTimer, [this]()
    {
        SceneManager::ChangeScene(Define::Scene_GameClear);
    },
        0,
        false,
        3.0f);

    TimerManager::GetInstance().ClearTimer(gameOverTimer);
    TimerManager::GetInstance().SetTimer(gameOverTimer, [this]()
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::PointDiffuse), true, 2.0f, 1.0f);
    },
    0,
    false,
    1.0f);

    
}


void GamePlayManager::PlayBossMode()
{
    if (WeakObjectPtr<gameObject> triggerBox = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"EnemySpawnTriggerBox"))
    {
        if (triggerBox.expired()) return;
        if (EnemySpawnTriggerBox* es = triggerBox->GetComponent<EnemySpawnTriggerBox>())
        {
            es->SetSpawnable(false);
            es->SpawnBossAt(FVector2(4800, 0));

            TimerManager::GetInstance().SetTimer(bossSpawnTimer, 
                [this]()
            {
					/*FVector2 center = m_player->GetPosition();
					FVector2 randomPoint = FRandom::GetRandomPointInCircle2D(center.x, center.y, 20);
					EnemySpawnTriggerBox::SpawnEnemyAt(0, randomPoint + FVector2(600, 0));*/
                    if (GamePlayManager::GetInstance().IsPaused()) return;
                    // 보스 드론 소환
                    FVector2 randomPoint = FRandom::GetRandomPointInTorus2D(0, 0, 300, 600);
                    EnemySpawnTriggerBox::SpawnBossDroneAt(randomPoint + FVector2(300, 0));
                    
                    // 일반 몬스터도 소환 (랜덤 타입)
                    int enemyType = FRandom::GetRandomInRange(0, 3); // 0~3 타입의 적
					FVector2 enemySpawnPos = FRandom::GetRandomPointInCircle2D(0, -150, 250);
					//FVector2 enemySpawnPos = FVector2(0,-150);
                    if (gameObject* player = GamePlayManager::GetInstance().GetPlayer())
                    {
                        FVector2 pos = player->GetPosition();
                        EnemySpawnTriggerBox::SpawnEnemyInBossModeAt(1000, FVector2(pos.x,0) + enemySpawnPos + FVector2(2200, 0));
                    }
            },
                3.0f,
                true,
                1.0f);
        }
    }
}

void GamePlayManager::BackNormalMode()
{
    if (WeakObjectPtr<gameObject> triggerBox = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"EnemySpawnTriggerBox"))
    {
        if (triggerBox.expired()) return;
        if (EnemySpawnTriggerBox* es = triggerBox->GetComponent<EnemySpawnTriggerBox>())
        {
            es->SetSpawnable(true);
            TimerManager::GetInstance().ClearTimer(bossSpawnTimer);
        }
    }
}

void GamePlayManager::SpawnVignette(float durationSec, float maxAlpha)
{
    if (auto go = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(L"VignetteOverlay"))
    {
        SceneManager::GetInstance().GetCamera()->AddChildObject(go);
        auto* vig = go->AddComponent<VignetteWidgetScript>();
        vig->SetDuration(durationSec);
        vig->SetMaxEdgeAlpha(maxAlpha);
    }
}

void GamePlayManager::SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha)
{
    if (auto go = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(L"BlackOutOverlay"))
    {
        SceneManager::GetInstance().GetCamera()->AddChildObject(go);
        auto* bo = go->AddComponent<BlackOutWidgetScript>();
        bo->SetDuration(durationSec);
        bo->SetMaxEdgeAlpha(maxAlpha);
        bo->SetMode(static_cast<EBlackOutLightingMode>(modeIndex));
        bo->UseCrossFade(useCrossFade);
    }
}

void GamePlayManager::PauseGame()
{
    if (IsPaused()) return;
    ApplyPauseTimescale(true);
    SetState(EGameRunState::Paused);
}

void GamePlayManager::ResumeGame()
{
    if (!IsPaused()) return;
    ApplyPauseTimescale(false);
    SetState(EGameRunState::InGame);
}

void GamePlayManager::TogglePause()
{
    if (IsPaused()) ResumeGame();
    else            PauseGame();
}

void GamePlayManager::EndGameAsGameOver()
{
    // 타임스케일은 건드리지 않음(연출/UI 필요 시 외부에서 제어)
    SetState(EGameRunState::GameOver);
}

void GamePlayManager::EndGameAsVictory()
{
    SetState(EGameRunState::Victory);
}

// ===== 씬 전환 (UI가 연출/타이머/실제 ChangeScene 수행) =====
void GamePlayManager::MarkTransitionBegin(const std::wstring& nextScene)
{
    m_PendingScene = nextScene;
    SetState(EGameRunState::Transitioning);
}

void GamePlayManager::MarkTransitionComplete()
{
    // 실제 씬 교체는 UI/외부에서 이미 수행했다고 가정
    if (!m_PendingScene.empty()) {
        m_CurrentScene = m_PendingScene;
        m_PendingScene.clear();
    }
    // 전환 완료 후 상태 복귀: 로딩 중이 아니면 InGame으로
    if (m_State == EGameRunState::Transitioning)
        SetState(EGameRunState::InGame);
}

void GamePlayManager::MarkLoadingBegin()
{
    SetState(EGameRunState::Loading);
}

void GamePlayManager::MarkLoadingComplete()
{
    // 로딩 완료 후 기본은 InGame로(필요 시 UI에서 Pause/Start를 별도로 호출 가능)
    if (m_State == EGameRunState::Loading)
        SetState(EGameRunState::InGame);
}

// 모든 내부 타이머 클리어 (씬 전환 종료 시 호출)
void GamePlayManager::ReleaseTimers()
{
    auto& TM = TimerManager::GetInstance();
    TM.ClearTimer(gameOverTimer);
    TM.ClearTimer(gameOverTransitionTimer);
    TM.ClearTimer(bossSpawnTimer);
    TM.ClearTimer(m_bossFlickerTimer);
}
