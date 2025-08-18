#pragma once
#include "Component/ScriptComponent.h"

class BikeMovementScript;

class PlayerMovement : public ScriptComponent
{
public:
	float bonusSpeed = 240.0f;
	float maxSpeed = 360.0f;
	float minSpeed = 150.0f;

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void SetBikeMovment(BikeMovementScript* _bikeMovement) { bMovement = _bikeMovement; }
	void SetInitSpeed(float _speed) { initSpeed = _speed; }
	float GetInitSpeed() { return initSpeed; }

	// 플레이어 zPos 클램프 설정
	void SetZClamp(float _min, float _max);

	// 부스트 설정
	void SetBoost(bool _val);

	// 표시속도 가져오기
	float GetRealSpeed();

	// 충돌시 InitSpeed로 전환
	void CrashSlow();

	// 멈춰!
	void Stop();

	// 시작해
	void Start();
private:
	BikeMovementScript* bMovement;
	float initSpeed;					// 기본 스피드
	float decVal = 0.5f;				// 감속값
	FVector2 moveDir = { 0.0f, 0.0f };	// 가속방향
	float playerDeltaSeconds;
	bool isBoosting = false;

	bool isMoving = true;

	float minZ = -220.0f;
	float maxZ = 380.0f;

	void Input();
};

