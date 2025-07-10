#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <Object/UObject.h>
#include <Experimental/Physics/AABB.h>
/*
* @briefs : �������� ��� Collider�� �浹�� �����ϴ� �ý����Դϴ�.
* @details :
*	Scene�� Initialize���� Update�Լ��� UpdateTaskManager�� ������ݴϴ�.
*	������Ʈ �Լ��� ���� ���� ITickable�� ��ӹް�
*	������Ʈ �ڵ��� ����ϱ� ���� UObject�� ��ӹ޽��ϴ�.
*	���� ��ϵǾ� �ִ� ��� Collider�� �浹 �˻縦 �� Update���� �����մϴ�.
*/

class Collider;
class CollisionSystem : public ITickable, public UObject, public Singleton<CollisionSystem>
{
public:
	CollisionSystem();
	~CollisionSystem();

public:
	void Regist(const WeakObjectPtr<Collider>& _component);
	void UnRegist(WeakObjectPtr<Collider>&& _component);
	void UnRegistAll();
	virtual void Update(const float& deltaSeconds) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

protected:
	std::vector<WeakObjectPtr<Collider>> m_AABBs;
};