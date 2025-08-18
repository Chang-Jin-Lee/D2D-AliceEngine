#include "BikeMovementScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>
#include <Component/SkewTransform.h>
#include <Animation/AnimatorInstance.h>
#include "Component/Collider.h"
#include "../Player/PlayerManager.h"

void BikeMovementScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);

	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
	REGISTER_SCRIPT_TICK(Update);
}

void BikeMovementScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성
}

void BikeMovementScript::Update(const float& deltaSeconds)
{
  	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
	// 슬로우/부스트 효과 시간 감소
	if (m_modifierDuration > 0.0f)
	{
		m_modifierDuration -= deltaSeconds;
		if (m_modifierDuration <= 0.0f)
		{
			m_speedModifier = 1.0f;
			m_modifierDuration = 0.0f;
		}
	}

	// 충돌 반응 처리
	if (m_hitReaction)
	{
		m_hitTimer -= deltaSeconds;
		if (m_hitTimer <= 0.0f)
		{
			m_hitReaction = false;
			m_hitTimer = 0.0f;
		}
		return; // 움찔 중일 땐 정지
	}

	// 가속도 적용
	m_currentSpeed += m_acceleration * deltaSeconds;
	m_currentSpeed = min(m_currentSpeed, m_maxSpeed);

	// 최종 속도 = 현재속도 * 효과배율
	float finalSpeed = m_currentSpeed * m_speedModifier;
	m_prevMoveAmount = finalSpeed;
	// 오른쪽으로 이동
	if (auto transform = owner->transform())
		transform->AddPosition(finalSpeed * deltaSeconds, 0);


	// 점프 중 물리 처리
	if (m_isJumping)
	{
		// 수직 속도에 중력 적용
		m_jumpVelocity += m_jumpGravity * deltaSeconds;

		float dy = m_jumpVelocity * deltaSeconds;

		if (auto tr = owner->GetComponent<SkewTransform>())
		{
			FVector2 pos = tr->GetRealPos();
			pos.y += dy;

			// 착지 체크
			if (pos.y <= m_groundY)
			{
				// 콜라이더 돌려놓기
				owner->GetComponent<Collider>()->SetLayer(prevCollLayer);

				// 부스터 설정
				PlayerManager::instance->Boost(3.0f, false);

				pos.y = m_groundY;
				m_isJumping = false;
				m_jumpVelocity = 0.0f;
				owner->transform()->SetRotation(0.0f);

				/*if (auto anim = owner->GetComponent<AnimatorInstance>())
				{
					anim->m_layer = m_jumpPrevLayer;
				}*/
			}

			


			if (auto transform = m_owner->transform()) {
				//transform->AddPosition((finalSpeed + m_jumpWeightX) * deltaSeconds, 0);

				pos.x += (m_jumpWeightX * 3.0f) * deltaSeconds;
			}

			tr->SetRealPos(pos);
		}
	}
}

void BikeMovementScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void BikeMovementScript::Awake()
{
}

void BikeMovementScript::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();
	m_currentSpeed = m_initialSpeed;
	if (m_owner && m_owner->transform())
		m_groundY = m_owner->transform()->GetPosition().y;

}

void BikeMovementScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void BikeMovementScript::OnDestroy()
{
}

void BikeMovementScript::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D 호출됨\n");

	m_hitReaction = true;
	m_hitTimer = m_hitReactionTime;
}

void BikeMovementScript::OnCollisionStay2D(Collision2D* collider)
{
	/*std::cout << "OnCollisionStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D 호출됨\n");*/
}

void BikeMovementScript::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D 호출됨\n");
}

void BikeMovementScript::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void BikeMovementScript::OnTriggerStay2D(Collider* collider)
{
	/*std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");*/
}

void BikeMovementScript::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void BikeMovementScript::Input()
{
	// 여기에 Input에 대한 로직 작성
}


void BikeMovementScript::Jump()
{
	// 점프 입력 처리
	if (!m_isJumping && !m_hitReaction)
	{
		// 콜라이더 쌉무시
		prevCollLayer = owner->GetComponent<Collider>()->GetLayer();
		owner->GetComponent<Collider>()->SetLayer(10);

		m_isJumping = true;
		m_groundY = owner->GetComponent<SkewTransform>()->GetRealPos().y;
		m_jumpVelocity = m_jumpInitialVelocity;

		/*if (auto anim = owner->GetComponent<AnimatorInstance>())
		{
			m_jumpPrevLayer = anim->m_layer;
			anim->m_layer = 70000;
		}*/

		// 회전 적용 (45도)
		if (auto tr = m_owner->transform())
			tr->SetRotation(45.0f);
	}
}

void BikeMovementScript::ApplySlow(float slowFactor, float duration)
{
	m_speedModifier = slowFactor;      // 예: 0.5f → 절반 속도
	m_modifierDuration = duration;
}

void BikeMovementScript::ApplyBoost(float boostFactor, float duration)
{
	m_speedModifier = boostFactor;     // 예: 1.5f → 50% 증가
	m_modifierDuration = duration;
}
