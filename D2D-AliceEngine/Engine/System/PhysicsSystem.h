#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <Object/UObject.h>
#include <Component/Rigidbody2D.h>
/*
* @briefs : �������� ��� RigidBody2D�� ����ϴ� �ý����Դϴ�.
* @details :
*/

class PhysicsSystem : public ITickable, public UObject, public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

public:
	void Regist(const WeakObjectPtr<Rigidbody2D>& _component);
	void UnRegist(WeakObjectPtr<Rigidbody2D>&& _component);
	void UnRegistAll();
	virtual void Update(const float& deltaSeconds) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	std::unordered_set<Rigidbody2D*> collidedBodies;
protected:
	std::vector<WeakObjectPtr<Rigidbody2D>> m_rigidBodies;
};