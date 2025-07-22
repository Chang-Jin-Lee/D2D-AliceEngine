#pragma once
#include <Core/ObjectHandler.h>
#include <Experimental/Physics/AABB.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

/*
* @briefs : 충돌 검사를 위한 스태틱 함수가 모여있는 클래스입니다
* @detail : 현재는 BruteForce, SweepAndPrune을 제공합니다. 
*/

namespace Physics
{
	// 충돌 검사 스태틱 함수들이 모여있는 클래스.
	class FCollisionDetector
	{
	public:
		FCollisionDetector() {};
		~FCollisionDetector() {};

		static void BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static std::unordered_set<Rigidbody2D*> SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects);
		static bool CheckCollisionCondition(WeakObjectPtr<Collider>& _object);
		static void LoadPreviousCollisions(); // 이전 충돌 정보 가져오기. OnCollisionStay를 위한 함수
		static bool CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static bool IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b);
		static void PushOverlappedArea(Collider* a, Collider* b);
		static void PushOverlappedAreaNoMass(Collider* a, Collider* b);
		static bool LineAABBIntersect(const FVector2& p0, const FVector2& p1, const FAABB& aabb, FVector2& outHitPos);

		static float CalcXOverlap(const FAABB& a, const FAABB& b); // x축 겹침 길이 계산
		static float CalcYOverlap(const FAABB& a, const FAABB& b); // y축 겹침 길이 계산
		static float Width(const FAABB& box); // aabb 전체 크기
		static bool IsOnGround(const FAABB& above, const FAABB& below, float threshold = 0.3f); // Ground 상태 판정
		static void SavePreviousCollisionData(Collider* src, Collider* tar, std::unordered_set<Rigidbody2D*>& overlappedRigdBodies);
	};
}

