#include "LocationIndicatorWidgetScript.h"

#include <Component/SpriteRenderer.h>
#include <Component/ProgressBarComponent.h>
#include <Component/InputComponent.h>
#include <Helpers/CoordHelper.h>
#include <Define/Define.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Object/gameObject.h>
#include <Component/Effect/ParticleComponent.h>
#include <cmath>

void LocationIndicatorWidgetScript::Initialize()
{
	__super::Initialize();
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
	
	// 데모 시간 초기화
	m_demoTime = 0.0f;
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
	// 시간 기반 진행 값 계산 (0..1) : ping-pong 동작 지원
	m_demoTime += deltaSeconds * m_demoSpeed;
	float cycle = fmodf(m_demoTime, 1.0f);
	float progress = m_pingPong ? (cycle <= 0.5f ? (cycle * 2.0f) : (1.0f - (cycle - 0.5f) * 2.0f))
		                        : cycle;

	// ProgressBar 갱신
	if (m_barFill)
		m_barFill->SetProgress(progress);
	
	// 마커 X 위치를 바의 좌우 폭에 맞춰 보간
	if (!m_barEmpty) return;
	const float barW = m_barEmpty->GetBitmapSizeX();
	const float halfW = barW * 0.5f; // 현재는 스케일 1.0 가정
	const float xOnBar = -halfW + (2.0f * halfW) * progress;
	const float xBossBar = -halfW + (2.0f * halfW) * bossSpawnRelativeRatio;
	if (m_marker)
	{
		m_marker->SetRelativePosition(FVector2(xOnBar, m_barY));
		if (bBossState && m_enemyMarker)
			m_enemyMarker->SetRelativePosition(FVector2(xBossBar, m_barY));
	}

	// 트레일 이펙트
	if (owner)
	{
		m_trailAccumulator += deltaSeconds;
		const float spawnRate = 2.0f; // 초당 2개
		const float interval = 1.0f / spawnRate;
		while (m_trailAccumulator >= interval)
		{
			m_trailAccumulator -= interval;
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
}


