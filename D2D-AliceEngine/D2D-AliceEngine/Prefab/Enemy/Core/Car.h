#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimationController.h"
#include <Manager/TimerManager.h>

// Forward declarations
class EnemyStatScript;

class Car : public ScriptComponent
{
public:
	Car() {}
    Car(bool _bSkewed) { bSkewed = _bSkewed; }
	virtual ~Car() {}

	void Initialize() override;
	void OnStart() override;
    void OnDestroy() override;
    void Update(const float& deltaSeconds) override;
	virtual void OnTriggerEnter2D(Collider* collider) override;

	bool GetSkewed() const { return bSkewed; }
	void SetSkewed(const bool value) { bSkewed = value; }

    // 총알 등으로 사망 시 지연 파괴
    void DelayDestroy();

	// 총알 콜라이더 삭제
	void DestroybColl();
protected:
	AnimatorController animController;

	FTimerHandle timer;
    // 지연 파괴 상태
    bool m_isDelaying = false;
    bool m_skipGhostOnDestroy = false;

    // 페이드 제어
    bool m_isFading = false;
    float m_fadeDuration = 1.0f;
    float m_fadeElapsed = 0.0f;
    class SpriteRenderer* m_fadeTargetSR = nullptr; // 고스트 또는 폴백 대상
    class gameObject* m_ghostGO = nullptr;          // 생성된 고스트 오브젝트

    bool bSkewed = true;

	std::vector<gameObject*> colObjs;

private:
	// 부드러운 흔들림(바운스) 파라미터
	float m_bobTime = 0.0f;   // 누적 시간
	float m_bobAmpX = 20.0f;  // X 진폭(픽셀)
	float m_bobAmpY = 14.0f;  // Y 진폭(픽셀)
	float m_bobFreqX = 0.45f;  // X 진동(Hz)
	float m_bobFreqY = 0.72f;  // Y 진동(Hz) - 서로 다르게 해서 패턴 반복 방지
	float m_bobPhase = 1.37f;  // 위상 차이(라디안)


	FTimerHandle sweepTimer;
	bool   bSweepRunning = false;

	float  sweepMaxAngle = 60.0f;  // 예: 60 -> -60도 ~ +60도
	float  sweepStepDeg = 10.0f;  // 예: 10도 간격
	float  sweepInterval = 0.2f;   // 0.2초마다 한 발
	float  sweepRange = 1200.0f;

	int    sweepIndex = 0;
	int    sweepSteps = 0;

	FVector2 sweepBaseDir{ 0,0 };
	FVector2 sweepBasePos{ 0,0 };
};

