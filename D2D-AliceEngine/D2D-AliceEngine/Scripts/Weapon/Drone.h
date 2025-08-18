#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>
#include "../Audio.h"

struct FDroneSpritePath
{
	std::wstring body;
	std::wstring arm;

	FDroneSpritePath() : body(L""), arm(L"") {}
	FDroneSpritePath(std::wstring _body, std::wstring _arm) : body(_body), arm(_arm) {}
	void operator= (const FDroneSpritePath& other)
	{
		body = other.body;
		arm = other.arm;
	}
};

enum class EDroneType
{
	Player,
	Enemy,
	BossSpawn,
	Boss
};
class gameObject;
class SpriteRenderer;
class Drone : public ScriptComponent
{
public:
	Drone(FDroneSpritePath path);
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void Floating(const float& deltaSeconds, const FVector2& dirNormal);
	void AttackAction(const FVector2& bodyPos, const FVector2& worldMousePos, const FVector2& dirNormal);
	void FireOneBurstShot();
	void UpdateBossDroneMovement(const float& deltaSeconds); // 보스 드론 독립 이동 함수 추가
	void LateUpdate(const float& deltaSeconds);
	// 사망 연출: 스프라이트를 페이드 아웃시키고 1초 뒤 삭제, 그동안 공격 금지
	void DelayDestroy();

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	EDroneType GetDroneType() const{ return droneType;}
	void SetDroneType(EDroneType type){ droneType = type;}
	float GetAttackDelay() const { return attackDelay; }
	void SetAttackDelay(float val) { attackDelay = val; }
	
	// 데미지 관련 함수 추가
	float GetDamage() const { return damage; }
	void SetDamage(float val) { damage = val; }

	float GetBulletSpeed() const { return m_bulletSpeed; }
	void SetBulletSpeed(const float value) { m_bulletSpeed = value; }

	gameObject* m_player = nullptr;
	gameObject* m_owner = nullptr;

	SpriteRenderer* body = nullptr;
	SpriteRenderer* arm = nullptr;

	FVector2 initBodyPos = FVector2(-50.0f, 40.0f);
	FVector2 initBodySize = FVector2(0.7f, 0.7f);

	FTimerHandle timer;
	bool bCanFire = true;
	bool bWaitForSecond = true;
	bool bDelayDestroying = false;
  // 공개 페이드 파라미터 (인스턴스별 조절 가능)
	float fadeDuration = 1.0f;  // 페이드 시간(초)
	float fadeElapsed  = 0.0f;  // 진행 시간(초)

	// Recoil(반동) 파라미터
	FVector2 recoilOffset = FVector2(0.0f, 0.0f);   // 현재 누적 반동 오프셋 (발사 방향의 반대)
	FVector2 recoilDir    = FVector2(0.0f, 0.0f);   // 마지막 반동 방향(단위 벡터)
	float    recoilMaxDistance = 24.0f;             // 최대 밀려나는 거리(px)
	float    recoilImpulse     = 6.0f;              // 1회 발사 시 추가되는 반동 거리(px)
	float    recoilReturnSpeed = 22.0f;             // 원위치 복귀 속도(px/s)

	// 위아래 진폭과 속도
	float elapsed = 0.0f;
	float duration = 1.0f; // 위→아래 한 번 이동하는데 걸리는 시간
	float startY = 0.0f;
	float endY = 7.0f;    // 위로 5만큼 이동
	bool goingUp = true;

	FDroneSpritePath spritePath;
	EDroneType droneType = EDroneType::Enemy;
	float attackDelay = 0.5f;
	float damage = 5.0f; // 기본 데미지 값 추가

	// 상대방 바라보는 팔 회전 추가 각도 - 이미지가 회전되어 있을 경우 추가.
	float armDegree = 0.0f;

	// 플레이어 원형으로 쏠 변수
	FTimerHandle burstTimer;
	int   burstRemaining = 0;
    float burstInterval = 0.2f;    // 연속 발사 간격(초)
	float spreadRadius = 340.0f;  // 플레이어 주변 랜덤 조준 반경
	
	// 보스 드론 독립 이동 관련 변수
	FVector2 m_baseOffset = FVector2(400.0f, 0.0f);      // 플레이어로부터의 기본 상대 위치
	FVector2 m_movementOffset = FVector2(0.0f, 0.0f);    // 기본 위치에서의 추가 이동 오프셋
	float m_movementSpeed = 80.0f;                        // 이동 속도
	float m_movementRadius = 120.0f;                      // 이동 반경 (카메라 화면 안에서)
	float m_movementAngle = 0.0f;                         // 현재 이동 각도
	float m_movementAngleSpeed = 90.0f;                   // 각도 변화 속도 (도/초)
	float m_movementTimer = 0.0f;                         // 이동 패턴 변경 타이머
	float m_patternChangeInterval = 3.0f;                  // 패턴 변경 간격 (초)


	// SweepShoot, 쓸듯이 나가는 탄막 ─────────────────────────────────────────────────────────
	FTimerHandle sweepTimer;
	bool   bSweepRunning = false;

	float  sweepMaxAngle = 60.0f;  // 예: 60 -> -60도 ~ +60도
	float  sweepStepDeg = 10.0f;  // 예: 10도 간격
	float  sweepInterval = 0.1f;   // 0.2초마다 한 발
	float  sweepRange = 1200.0f;

	int    sweepIndex = 0;
	int    sweepSteps = 0;

	FVector2 sweepBaseDir{ 0,0 };
	FVector2 sweepBasePos{ 0,0 };

	enum class EBossPattern { None, Single, Burst3, Sweep };

	EBossPattern currentPattern = EBossPattern::None;
	bool  bPatternRunning = false;   // 어떤 패턴이든 동작 중이면 true
	bool  bBurstRunning = false;   // 3연사 진행 중 관리용

	// 패턴 확률 (합계 아무 값이어도 됨, 실행 시 정규화)
	float probSingle = 0.43f;
	float probBurst3 = 0.34f;
	float probSweep = 0.23f;

private:
	float m_bulletSpeed{1};
};
