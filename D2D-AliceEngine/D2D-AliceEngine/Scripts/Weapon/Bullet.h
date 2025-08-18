#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>
#include "Drone.h"
#include <vector>

enum class EBulletType
{
	Linear,
	BezierCurve,
	SinCurve,
	CatmullRom
};

class gameObject;
class Bullet : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void UpdatePositionByType(const float& deltaSeconds);
	void UpdateCameraCulling();
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	gameObject* m_owner;

	EDroneType GetDroneType() const { return droneType; }
	void SetDroneType(EDroneType type) { droneType = type; }
	const std::wstring& GetSpritePath() const { return spritePath; }
	void SetSpritePath(const std::wstring& path) { spritePath = path; }
	
	// 데미지 관련 함수 추가
	float GetDamage() const { return damage; }
	void SetDamage(float val) { damage = val; }

public:
	FVector2 P0, P1, P2;
	float time = 0.0f;
	float duration = 1.0f; // 총알 비행 시간 (초)
	bool isActive = true;

	FVector2 inheritedVelocity; // 플레이어 속도 저장용
	FVector2 accumulatedOffset; // 속도 누적 위치
	bool bBezierFinished = false;

	FVector2 moveDir; // 총알의 직진 방향 (Normalized)
	FVector2 startPos;
	FVector2 currentPos;
	float moveSpeed = 8.1f; // 총알 속도

	float waveAmplitude = 0.2f; // 흔들림 크기
	float waveFrequency = 2.0f;  // 흔들림 속도

	// Catmull-Rom spline data
	std::vector<FVector2> catmullPoints; // 최소 4개: P0,P1,P2,P3 ...
	float splineT = 0.0f;                // 현재 세그먼트 진행도 [0,1]
	float splineDuration = 1.0f;         // 전체 곡선 소요 시간(초)
	int   splineSegment = 0;             // 현재 세그먼트 인덱스 (0..numSeg-1)
	bool  bSplineFinished = false;

	EBulletType bulletType = EBulletType::BezierCurve;
	EDroneType droneType = EDroneType::Player; // 총알 타입
	std::wstring spritePath = L"wallet.png";
	float damage = 5.0f; // 데미지 값 추가

	FTimerHandle handle;
	bool bCameraCulling = false;
};

