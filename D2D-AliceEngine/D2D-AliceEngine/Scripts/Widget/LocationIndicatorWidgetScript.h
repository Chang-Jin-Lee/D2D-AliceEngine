#pragma once

#include <Component/ScriptComponent.h>

class SpriteRenderer;
class ProgressBarComponent;

// 카메라 위에 고정되는 진행도 HUD
class LocationIndicatorWidgetScript : public ScriptComponent
{
public:
    void Initialize() override;
    void Update(const float& deltaSeconds) override;
    void OnStart() override;
    void OnEnd() override;
    void OnDestroy() override;

    // 진행 시작점(플레이 중 첫 탐지된 플레이어 X) 강제 설정이 필요하면 외부에서 지정 가능
    void SetStartX(float x) { m_startX = x; m_hasStartX = true; }

	bool GetBossState() const { return bBossState; }
	void SetBossState(const bool& value) { bBossState = value; }

private:
    // UI 구성요소
    SpriteRenderer*         m_barEmpty = nullptr;   // 배경(빈 바)
    ProgressBarComponent*   m_barFill  = nullptr;   // 채워지는 컬러 바
    SpriteRenderer*         m_marker   = nullptr;   // 현재 위치 표시용 글로우/마커
    SpriteRenderer*         m_enemyMarker   = nullptr;   // 현재 위치 표시용 글로우/마커
    gameObject* enemyIndecatorObj;
    bool bossSpawned{false};

    // 배치/레이어
    int   m_baseLayer = 50000; // HUD 기준 상대 레이어 (StageWidgetScript를 참고해 여유있게 높임)
    float m_barY = 0.0f;       // 화면 상단 오프셋 위치

    // 진행 계산
    bool  m_hasStartX = false;
    float m_startX = 0.0f;     // 시작 X (초기 플레이어 X로 자동 캡처)

    // 마커 연출
    float m_effectTime = 0.0f; // 펄싱 애니메이션 누적 시간

    // 내부 유틸
    void updateProgressAndMarker(const float& deltaSeconds);

    float m_trailAccumulator{ 0 };

    bool bBossState = false;

    float bossSpawnRelativeRatio = 0.7f;
    
};


