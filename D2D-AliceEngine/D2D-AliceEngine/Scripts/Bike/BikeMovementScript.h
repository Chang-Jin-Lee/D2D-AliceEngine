#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class BikeMovementScript : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnCollisionEnter2D(Collision2D* collider) override;
	virtual void OnCollisionStay2D(Collision2D* collider) override;
	virtual void OnCollisionExit2D(Collision2D* collider) override;
	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	// 속도 효과 적용 함수
	void ApplySlow(float slowFactor, float duration);
	void ApplyBoost(float boostFactor, float duration);

	// 현재 속도 출력
	float GetCurrSpeed() { return m_currentSpeed; }

	// 현재 속도변화 출력
	float GetSpeedModifier() { return m_speedModifier; }

	void Input();

	// Owner
	// Initial Speed
	float GetInitialSpeed() const { return m_initialSpeed; }
	void SetInitialSpeed(float speed) { m_initialSpeed = speed; }
	void AddInitialSpeed(float delta) { m_initialSpeed += delta; }

	// Max Speed
	float GetMaxSpeed() const { return m_maxSpeed; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	void AddMaxSpeed(float delta) { m_maxSpeed += delta; }

	// Acceleration
	float GetAcceleration() const { return m_acceleration; }
	void SetAcceleration(float acceleration) { m_acceleration = acceleration; }
	void AddAcceleration(float delta) { m_acceleration += delta; }

	// Current Speed
	float GetCurrentSpeed() const { return m_currentSpeed; }
	void SetCurrentSpeed(float speed) { m_currentSpeed = speed; }
	void AddCurrentSpeed(float delta) { m_currentSpeed += delta; }

	// Hit Reaction
	bool IsHitReaction() const { return m_hitReaction; }
	void SetHitReaction(bool hit) { m_hitReaction = hit; }

	// Hit Reaction Time
	float GetHitReactionTime() const { return m_hitReactionTime; }
	void SetHitReactionTime(float time) { m_hitReactionTime = time; }
	void AddHitReactionTime(float delta) { m_hitReactionTime += delta; }

	// Hit Timer
	float GetHitTimer() const { return m_hitTimer; }
	void SetHitTimer(float time) { m_hitTimer = time; }
	void AddHitTimer(float delta) { m_hitTimer += delta; }

	// Speed Modifier
	float GetSpeedModifierValue() const { return m_speedModifier; }
	void SetSpeedModifierValue(float modifier) { m_speedModifier = modifier; }
	void AddSpeedModifierValue(float delta) { m_speedModifier += delta; }

	// Modifier Duration
	float GetModifierDuration() const { return m_modifierDuration; }
	void SetModifierDuration(float duration) { m_modifierDuration = duration; }
	void AddModifierDuration(float delta) { m_modifierDuration += delta; }

	// Prev Move Amount
	float GetPrevMoveAmount() const { return m_prevMoveAmount; }
	void SetPrevMoveAmount(float amount) { m_prevMoveAmount = amount; }
	void AddPrevMoveAmount(float delta) { m_prevMoveAmount += delta; }

	// Jump
	void Jump();

private:
	gameObject* m_owner = nullptr;

	// 기본 속도 설정
	// 표시속도는 m_maxSpeed * 0.3f 로 쓰는게 타당하다 봅니다
	float m_initialSpeed = 200.0f;
	float m_maxSpeed = 300.0f;
	float m_acceleration = 40.0f;

	// 움직임 클램프
	float minZ = -220.0f;
	float maxZ = 380.0f;

	// 현재 속도
	float m_currentSpeed = 0.0f;

	// 충돌 반응
	bool m_hitReaction = false;
	float m_hitReactionTime = 0.2f;
	float m_hitTimer = 0.0f;

	// 속도 변화 효과
	float m_speedModifier = 1.0f;
	float m_modifierDuration = 0.0f;

	float m_prevMoveAmount = 0.0f;

	// ================== Jump 관련 ==============
	bool  m_isJumping   = false;   // 점프 중 여부
	float m_jumpWeightX = 50.0f;
	float m_jumpVelocity         = 0.0f;    // 현재 점프 속도 Y축
	float m_jumpInitialVelocity  = 600.0f;  // 초기 점프 속도 양수: 위 방향
	float m_jumpGravity          = -600.0f; // 중력 가속도 양수: 아래 방향
	float m_groundY              = 0.0f;    // 지면 Y 위치 기준
	int m_jumpPrevLayer = 0;

	int prevCollLayer = 0;
};

