#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <Object/UObject.h>
#include <Experimental/Physics/AABB.h>
#include <set>
/*
* @briefs : 전역으로 모든 Collider의 충돌을 감지하는 시스템입니다.
* @details :
*	Scene의 Initialize에서 Update함수를 UpdateTaskManager에 등록해줍니다.
*	업데이트 함수를 쓰기 위해 ITickable을 상속받고
*	오브젝트 핸들을 사용하기 위해 UObject를 상속받습니다.
*	현재 등록되어 있는 모든 Collider의 충돌 검사를 매 Update마다 실행합니다.
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

public:
	std::vector<WeakObjectPtr<Collider>> m_colliders;

	// 충돌 상태 저장용 (주소가 작은 쪽이 first)
	std::set<std::pair<Collider*, Collider*>> previousCollisions;
	std::set<std::pair<Collider*, Collider*>> currentCollisions;
};