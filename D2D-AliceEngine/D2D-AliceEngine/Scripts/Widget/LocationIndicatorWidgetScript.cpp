#include "LocationIndicatorWidgetScript.h"

#include <Component/SpriteRenderer.h>
#include <Component/ProgressBarComponent.h>
#include <Component/InputComponent.h>
#include <Helpers/CoordHelper.h>
#include <Define/Define.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <GameManager/GamePlayManager.h>
#include <Object/gameObject.h>
#include <Component/Effect/ParticleComponent.h>
#include <cmath>

void LocationIndicatorWidgetScript::Initialize()
{
    __super::Initialize();
    //REGISTER_SCRIPT_METHOD(OnStart);
}

void LocationIndicatorWidgetScript::OnStart()
{
    // 카메라에 붙여 HUD처럼 고정
    if(GetWorld())
        enemyIndecatorObj = GetWorld()->NewObject<gameObject>(L"enemyIndecatorObj");
    owner->SetScale(FVector2(1, 0.8f));
    owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

    // 상단 중앙에 진행바 배치
    m_barY = -Define::SCREEN_HEIGHT * 0.5f + 10.0f; // 화면 최상단에서 40px 내려옴

    // 빈 바 스프라이트
    m_barEmpty = owner->AddComponent<SpriteRenderer>();
    m_barEmpty->SetDrawType(Define::EDrawType::ScreenSpace);
    m_barEmpty->LoadData(L"UI\\Location_Indicator_Empty.png");
    {
        const float w = m_barEmpty->GetBitmapSizeX();
        const float h = m_barEmpty->GetBitmapSizeY();
        if (w > 0.f && h > 0.f)
        {
            // 원본 크기 사용. 필요 시 스케일 조정
            m_barEmpty->SetRelativeScale(FVector2(1.f, 1.f));
        }
    }
    m_barEmpty->SetRelativePosition(FVector2(0.0f, m_barY));
    m_barEmpty->SetLayer(Define::HUDLayer + 5); // HUD 위로 약간 올림

    // 채워지는 컬러 바 (ProgressBar)
    m_barFill = owner->AddComponent<ProgressBarComponent>();
    m_barFill->SetDrawType(Define::EDrawType::ScreenSpace);
    m_barFill->LoadData(L"UI\\Location_Indicator_Color.png");
    m_barFill->SetType(EProgressBarType::Linear);
    m_barFill->SetRelativePosition(FVector2(0.0f, m_barY));
    m_barFill->SetRelativeScale(FVector2(1.f, 1.f));
    m_barFill->SetProgress(0.0f);
    m_barFill->SetLayer( Define::HUDLayer + 6); // 빈 바보다 위

    // 현재 위치 마커 (컬러 텍스처 재활용 또는 Empty를 사용하고 Opacity/Pulse)
    m_marker = owner->AddComponent<SpriteRenderer>();
    m_marker->SetDrawType(Define::EDrawType::ScreenSpace);
    m_marker->LoadData(L"UI\\Location_Indicator_Color.png");
    // 마커는 좁은 폭으로 보이도록 스케일 축소 (세로만 조금 키움)
    {
        const float w = m_marker->GetBitmapSizeX();
        const float h = m_marker->GetBitmapSizeY();
        if (w > 0.f && h > 0.f)
        {
            // 가로 스케일을 매우 작게 하여 얇은 선 형태
            m_marker->SetRelativeScale(FVector2(0.02f, 1.1f));
        }
    }
    m_marker->SetRelativePosition(FVector2(0.0f, m_barY));
    m_marker->SetLayer(Define::HUDLayer + 7);

    if (bBossState)
    {
		m_enemyMarker = owner->AddComponent<SpriteRenderer>();
		m_enemyMarker->SetDrawType(Define::EDrawType::ScreenSpace);
		m_enemyMarker->LoadData(L"UI\\Location_Indicator_Color_Enemy.png");
		// 마커는 좁은 폭으로 보이도록 스케일 축소 (세로만 조금 키움)
		{
			const float w = m_enemyMarker->GetBitmapSizeX();
			const float h = m_enemyMarker->GetBitmapSizeY();
			if (w > 0.f && h > 0.f)
			{
				// 가로 스케일을 매우 작게 하여 얇은 선 형태
				m_enemyMarker->SetRelativeScale(FVector2(0.02f, 1.1f));
			}
		}
		m_enemyMarker->SetRelativePosition(FVector2(0.0f, m_barY));
		m_enemyMarker->SetLayer(Define::HUDLayer + 7);
    }
    

    // 시작 X 자동 설정
    if (!m_hasStartX)
    {
        if (gameObject* player = GamePlayManager::GetInstance().GetPlayer())
        {
            m_startX = player->GetPosition().x;
            m_hasStartX = true;
        }
    }
}

void LocationIndicatorWidgetScript::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);

    updateProgressAndMarker(deltaSeconds);

    // 간단한 펄스 연출로 마커 강조
    m_effectTime += deltaSeconds;
    float pulse = 0.7f + 0.3f * sinf(m_effectTime * 6.0f);
    if (m_marker)
    {
        m_marker->SetOpacity(pulse);
        if (bBossState)
            m_enemyMarker->SetOpacity(pulse);
    }
}

void LocationIndicatorWidgetScript::OnEnd() {}
void LocationIndicatorWidgetScript::OnDestroy() {}

void LocationIndicatorWidgetScript::updateProgressAndMarker(const float& deltaSeconds)
{
    // 진행 계산: (currentX - startX) / (stopX - startX)
    GamePlayManager& gm = GamePlayManager::GetInstance();
    const int stopX = gm.GetStopXAxis();
    gameObject* player = gm.GetPlayer();
    if (!player || !m_barFill || !m_barEmpty) return;

    if (!m_hasStartX)
    {
        m_startX = player->GetPosition().x;
        m_hasStartX = true;
    }

    const float currentX = player->GetPosition().x;
    const float denom = (static_cast<float>(stopX) - m_startX);
    float progress = 0.0f;
    if (fabsf(denom) > 0.0001f)
        progress = (currentX - m_startX) / denom;

    // 0..1 클램프
    if (progress < 0.f) progress = 0.f;
    if (progress > 1.f) progress = 1.f;

    // ProgressBar 갱신
    m_barFill->SetProgress(progress);
   
    // 마커 X 위치를 바의 좌우 폭에 맞춰 보간
    // 빈 바 비트맵 폭 기준으로 좌측 끝(-w/2) ~ 우측 끝(+w/2)
    const float barW = m_barEmpty->GetBitmapSizeX();
    const float halfW = barW * 0.5f; // 현재는 스케일 1.0 가정
    const float xOnBar = -halfW + (2.0f * halfW) * progress;
    const float xBossBar = -halfW + (2.0f * halfW) * bossSpawnRelativeRatio;
    if (m_marker)
    {
        m_marker->SetRelativePosition(FVector2(xOnBar, m_barY));
        if (bBossState)
            m_enemyMarker->SetRelativePosition(FVector2(xBossBar, m_barY));
    }

    if (owner)
    {
        m_trailAccumulator += deltaSeconds;
        const float spawnRate = 2.0f; // 초당 60개
        const float interval = 1.0f / spawnRate;
        while (m_trailAccumulator >= interval)
        {
            m_trailAccumulator -= interval;
            //m_indicatorObject->SetPosition(FVector2(currentX, m_barY));
            if (ParticleComponent* particle = owner->GetComponent<ParticleComponent>())
            {
                particle->EmitLocationIndicator(FVector2(xOnBar, m_barY), 40);
            }
            else
            {
                auto* particle2 = owner->AddComponent<ParticleComponent>();
                particle2->LoadData(L"Effect/LocationIndicator.png");
                particle2->SetDrawType(Define::EDrawType::ScreenSpace);
                particle2->EmitLocationIndicator(FVector2(xOnBar, m_barY), 40);
            }
        }
    }

    if (bBossState && owner && enemyIndecatorObj)
    {
        if (owner->GetPosition().x < GamePlayManager::GetInstance().GetStopXAxis() * bossSpawnRelativeRatio)
        {
            if (ParticleComponent* particle = enemyIndecatorObj->GetComponent<ParticleComponent>())
            {
                particle->EmitLocationIndicator(FVector2(0, m_barY), 40, D2D1::ColorF(10.9f, 0.6f, 0.2f, 0.0f), D2D1::ColorF(21.0f, 0.0f, 0.0f, 0.9f));
            }
            else
            {
                auto* particle2 = enemyIndecatorObj->AddComponent<ParticleComponent>();
                particle2->LoadData(L"Effect/LocationIndicator.png");
                particle2->SetDrawType(Define::EDrawType::ScreenSpace);
            }
        }
        else
        {
            if (!bossSpawned)
            {
                GamePlayManager::GetInstance().PlayBossMode();
                bossSpawned = true;
            }
        }
    }
    
}


