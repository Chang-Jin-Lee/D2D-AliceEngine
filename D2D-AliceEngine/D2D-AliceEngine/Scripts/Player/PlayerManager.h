#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimatorInstance.h"
#include "Animation/AnimationController.h"
#include <Manager/TimerManager.h>

class PlayerManager : public  ScriptComponent
{
public:
	static PlayerManager* instance;

	void Initialize() override;
	void OnStart() override;
	void OnEnd() override;
	void Update(const float& deltaSeconds) override;

	// 플레이어를 잔상과 함께 서서히 사라지도록 파괴
	void DelayDestroy();

	// 플레이어 타임스케일 획득
	float GetTimeScale() { return playerTimeScale; }

	// 점프시키기
	void Jump();

	// 가속시키기
	void Boost(float _time = 5.0f, bool _battDec = true);

	// 싼데비슷한
	void Sande(float _time = 5.0f);
	
	// 무적상태 받아오기
	bool GetInvincible() { return isInvincible; }

	// 산데비스탄 상태출력
	bool GetSande() { return bSande; }

	// 부스트 상태출력
	bool GetBoost() { return bBoost; }
	
	// 충돌 감속효과
	void CrashSlow();

	// 플레이어 조작여부
	void SetMove(bool _val);

private:
	AnimatorInstance* animInstance = nullptr;
	AnimatorController animController;

	void Input();

	float playerTimeScale = 1.0f;

	// ===== DelayDestroy =====
	bool m_isDelaying = false;
	bool m_skipGhostOnDestroy = false;
	bool m_isFading = false;
	float m_fadeDuration = 1.0f;
	float m_fadeElapsed = 0.0f;
	class SpriteRenderer* m_fadeTargetSR = nullptr;
	class gameObject* m_ghostGO = nullptr;
	FTimerHandle m_fadeHandle;

	// 타이머
	bool bBoost = false;
	float boostTimer = 0.0f;
	float boostElipsed = 0.0f;

	bool bSande = false;
	float sandeTimer = 0.0f;
	float sandeElipsed = 0.0f;

	// 무적상태인가
	bool isInvincible = false;
};

