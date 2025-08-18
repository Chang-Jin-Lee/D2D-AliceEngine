#include "Enemy.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <System/ScriptSystem.h>
#include <Animation/AnimatorInstance.h>
#include <Animation/SpriteAnimationTextureLoader.h>
#include <Component/Animator.h>
#include <FSM/FiniteStateMachine.h>
#include <fsm/FSMState.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

void Enemy::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Enemy::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Enemy::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void Enemy::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Enemy::Awake()
{
}

void Enemy::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

	AnimatorController::LoadAnimatorController(L"DemoGame1/Ken_AnimController.json", animController);
	animInstance = m_owner->AddComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"DemoGame1/ken_sprites.json");
	animInstance->LoadAnimationClip(L"DemoGame1/ken_kick_anim.json");
	animInstance->LoadAnimationClip(L"DemoGame1/ken_idle_anim.json");
	animInstance->ApplyClipDurationsToStates();
	//animInstance->SetLooping(true);
	animInstance->Play();
	animInstance->SetLayer(3);
	animInstance->OnStart();
}

void Enemy::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void Enemy::OnDestroy()
{
}

void Enemy::Input()
{
	// 여기에 Input에 대한 로직 작성
}
