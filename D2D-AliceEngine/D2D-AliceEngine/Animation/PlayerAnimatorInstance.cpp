#include "pch.h"
#include "PlayerAnimatorInstance.h"
#include <Animation/AnimationController.h>

PlayerAnimatorInstance::PlayerAnimatorInstance()
{
}

PlayerAnimatorInstance::~PlayerAnimatorInstance()
{
}

void PlayerAnimatorInstance::Initialize()
{
	__super::Initialize();
}

void PlayerAnimatorInstance::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void PlayerAnimatorInstance::Release()
{
	__super::Release();
}

void PlayerAnimatorInstance::Render()
{
	__super::Render();
}

void PlayerAnimatorInstance::SetAnimatorController(AnimatorController* controller)
{
	__super::SetAnimatorController(controller);

	LoadSpriteSheet(L"Zero\\Zero_sprites.json");
	LoadAnimationClip(L"Zero\\Zero_idle_anim.json");
	LoadAnimationClip(L"Zero\\Zero_attack_anim.json");
	LoadAnimationClip(L"Zero\\Zero_walk_anim.json");
	//SetLooping(true);
	Play();

	// ��ü State�� ��ȸ�ϸ鼭 ����ϴ� AnimaitionClip�� ���̸� ������ State�� clipLength�� �����ϱ�
	for (auto& state : m_Controller->states)
	{
		state.clipLength = animationClips[state.motionName]->duration;
	}
}

void PlayerAnimatorInstance::ChangeState(const std::string& stateName)
{
	__super::ChangeState(stateName);
	// ����̸����� ���� ����� Animation Clip ����. 
	std::cout << "ChangeState "
		<< m_Controller->states[m_CurrentStateIndex].motionName << std::endl;
	nextAnimationClip = m_Controller->states[m_CurrentStateIndex].motionName;
}

void PlayerAnimatorInstance::OnStart()
{
	__super::OnStart();
}

void PlayerAnimatorInstance::OnExit()
{

}

