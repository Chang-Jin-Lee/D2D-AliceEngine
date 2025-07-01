#pragma once
#include <Object/UObject.h>
#include <Object/UObjectBase.h>

/*
* @brief : �� Ŭ������ ��ӹ޾Ƽ� IdleState, AttackState���� ���ϴ� �̸��� ������Ʈ�� ����ϴ�.
*/
class Animator;
class AnimationClip;
class FSMState : public UObjectBase
{
public:
	FSMState();
	~FSMState();
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

	void SetAnimator(Animator* _animator);
	void SetAnimationClip(AnimationClip* _motion);

private:
	Animator* animator;
	AnimationClip* motion;
};

