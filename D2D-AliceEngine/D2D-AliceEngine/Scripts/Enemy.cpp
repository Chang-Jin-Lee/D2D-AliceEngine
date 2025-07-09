#include "Enemy.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <System/ScriptSystem.h>

#include <Animation/TextureLoader.h>
#include <Component/Animator.h>
#include <FSM/FiniteStateMachine.h>
#include <fsm/FSMState.h>
#include <Component/Collider.h>

void Enemy::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Enemy::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Enemy::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�
}

void Enemy::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Enemy::Awake()
{
}

void Enemy::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();

	AnimatorController::LoadAnimatorController(L"Ken_AnimController.json", animController);
	animInstance = m_owner->AddComponent<EnemyAnimatorInstance>();
	animInstance->m_layer = 3;
	animInstance->SetAnimatorController(&animController);
	animInstance->OnStart();

	m_owner->AddComponent<Collider>()->SetBoxSize(FVector2(50, 80));
}

void Enemy::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Enemy::OnDestroy()
{
}

void Enemy::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
