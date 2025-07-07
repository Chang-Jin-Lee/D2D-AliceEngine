#pragma once
#include <Object/UObject.h>
#include <Object/UObjectBase.h>

/*
* @brief : �� Ŭ������ ��ӹ޾Ƽ� IdleState, AttackState���� ���ϴ� �̸��� ������Ʈ�� ����ϴ�.
*/
class Animator;
struct AnimationClip;
class FiniteStateMachine;
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
	void SetStateMachine(FiniteStateMachine* _owner);
	FiniteStateMachine* GetStateMachine();

protected:
	Animator* animator = nullptr;
	AnimationClip* motion = nullptr;
	FiniteStateMachine* owner = nullptr;
};

