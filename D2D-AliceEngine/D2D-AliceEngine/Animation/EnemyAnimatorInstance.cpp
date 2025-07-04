#include "pch.h"
#include "EnemyAnimatorInstance.h"
#include <Animation/AnimationController.h>

EnemyAnimatorInstance::EnemyAnimatorInstance()
{
}

EnemyAnimatorInstance::~EnemyAnimatorInstance()
{
}

void EnemyAnimatorInstance::Initialize()
{
	__super::Initialize();
}

void EnemyAnimatorInstance::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void EnemyAnimatorInstance::Release()
{
	__super::Release();
}

void EnemyAnimatorInstance::Render()
{
	__super::Render();
}

void EnemyAnimatorInstance::SetAnimatorController(AnimatorController* controller)
{
	__super::SetAnimatorController(controller);

	LoadSpriteSheet(L"ken_sprites.json");
	LoadAnimationClip(L"ken_kick_anim.json");
	LoadAnimationClip(L"ken_idle_anim.json");
	//SetLooping(true);
	Play();


	// ��ü State�� ��ȸ�ϸ鼭 ����ϴ� AnimaitionClip�� ���̸� ������ State�� clipLength�� �����ϱ�
	for (auto& state : m_Controller->states)
	{
		state.clipLength = animationClips[state.motionName]->duration;
	}
}

void EnemyAnimatorInstance::ChangeState(const std::string& stateName)
{
	__super::ChangeState(stateName);
	// ����̸����� ���� ����� Animation Clip ����. 
	std::cout << "ChangeState "
		<< m_Controller->states[m_CurrentStateIndex].motionName << std::endl;
	nextAnimationClip = m_Controller->states[m_CurrentStateIndex].motionName;
}

void EnemyAnimatorInstance::OnStart()
{
	__super::OnStart();
}

void EnemyAnimatorInstance::OnExit()
{

}

