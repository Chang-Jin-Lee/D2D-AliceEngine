#include "Animation.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <System/ScriptSystem.h>

#include <Helpers/TextureLoader.h>
#include <Component/Animator.h>

void Animation::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Animation::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Animation::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�


}

void Animation::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Animation::Awake()
{
}

void Animation::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();

	Texture = std::make_shared<SpriteSheet>();
	idle = std::make_shared<AnimationClip>();
	kick = std::make_shared<AnimationClip>();

	Animator::LoadSpriteSheet("ken_sprites.json", Texture);
	Animator::LoadAnimationClip("ken_kick_anim.json", kick, Texture);

	m_owner->transform()->SetPosition(50, 50);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(1.0f, 1.0f);
	m_owner->transform()->SetPivot(0.5f);

	m_animator = m_owner->AddComponent<Animator>();
	m_animator->SetAnimationClip(kick.get());
	m_animator->LoadSpriteRenderer(Texture);
	m_animator->Play();
}

void Animation::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Animation::OnDestroy()
{
}

void Animation::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
