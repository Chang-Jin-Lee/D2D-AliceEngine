#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/Rigidbody2D.h>
#include <Component/ScriptComponent.h>
#include <System/CollisionSystem.h>
#include "Collision2D.h"

void Physics::FCollisionDetector::BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if(objects[i].expired()) continue;
		if (objects[i]->dirty) continue;
		for (size_t j = i + 1; j < objects.size(); ++j)
		{
			if (objects[j].expired()) continue;

			if (IsOverlapped(objects[i],objects[j]))
			{
                PushOverlappedArea(objects[i].Get(), objects[j].Get());
				std::wcout << L"[충돌] " << objects[i]->GetName()<< " " << objects[j]->GetName() << " 겹침\n";
			}
		}
	}
}

std::unordered_set<Rigidbody2D*> Physics::FCollisionDetector::SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
    // Sweep and Prune을 위한 정렬
    std::sort(objects.begin(), objects.end(), &FCollisionDetector::CompareColliderMinX);
    
    std::unordered_set<Rigidbody2D*> overlappedRigdBodies;
    CollisionSystem::GetInstance().currentCollisions.clear();
    for (size_t i = 0; i < objects.size(); ++i)
    {
        auto src = objects[i];
		if (CheckCollisionCondition(src)) continue;

        for (size_t j = i + 1; j < objects.size(); ++j)
        {
			auto tar = objects[j];
			if (CheckCollisionCondition(tar)) continue;

            // Prune: 더 이상 겹칠 수 없으면 break
            if (tar->aabb.minVector.x > src->aabb.maxVector.x)
                break;

            // 실제 충돌 검사
            if (IsOverlapped(src, tar))
            {
				if (src.expired() || tar.expired()) continue;
                PushOverlappedArea(src.Get(), tar.Get());
				SavePreviousCollisionData(src.Get(), tar.Get(), overlappedRigdBodies);
            }
        }
    }
	LoadPreviousCollisions();

    return overlappedRigdBodies;
}

bool Physics::FCollisionDetector::CheckCollisionCondition(WeakObjectPtr<Collider>& _object)
{
	if (!_object) return true;
	if (_object->dirty) return true;
	if (_object->GetOwner()->transform()->bMoved == false) return true;
	return false;
}

void Physics::FCollisionDetector::LoadPreviousCollisions()
{
	// 이전 충돌 정보를 전부 검사하면서 현재 충돌정보에 없다면 그건 Exit이다.
	for (const auto& pair : CollisionSystem::GetInstance().previousCollisions)
	{
		if (CollisionSystem::GetInstance().currentCollisions.find(pair) == CollisionSystem::GetInstance().currentCollisions.end())
		{
			Collider* a = pair.first;
			Collider* b = pair.second;
			Collision2D collision2D;
			collision2D.collider = a;
			collision2D.otherCollider = b;
			if (a->GetOwner()) collision2D.rigidbody = a->GetOwner()->GetComponent<Rigidbody2D>();
			if (b->GetOwner()) collision2D.otherRigidbody = b->GetOwner()->GetComponent<Rigidbody2D>();
			collision2D.transform = a->GetTransform();

			std::vector<ScriptComponent*> scA, scB;
			if (a->GetOwner()) scA = a->GetOwner()->GetComponents<ScriptComponent>();
			if (b->GetOwner()) scB = b->GetOwner()->GetComponents<ScriptComponent>();
			for (auto sc : scA) sc->OnCollisionExit2D(&collision2D);
			for (auto sc : scB) sc->OnCollisionExit2D(&collision2D);
		}
	}
	CollisionSystem::GetInstance().previousCollisions = CollisionSystem::GetInstance().currentCollisions;
}

void Physics::FCollisionDetector::SavePreviousCollisionData(Collider* src, Collider* tar, std::unordered_set<Rigidbody2D*>& overlappedRigdBodies)
{
	// 사용할 변수들 선언부터 해주자.
	Collider* a = src;
	Collider* b = tar;
	Rigidbody2D* rbA = src->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = tar->GetOwner()->GetComponent<Rigidbody2D>();
	std::vector<ScriptComponent*> scA = a->GetOwner()->GetComponents<ScriptComponent>();
	std::vector<ScriptComponent*> scB = b->GetOwner()->GetComponents<ScriptComponent>();

	auto pair = std::minmax(a, b);
	CollisionSystem::GetInstance().currentCollisions.insert(pair);

	if (rbA) overlappedRigdBodies.insert(rbA);
	if (rbB) overlappedRigdBodies.insert(rbB);

	// 충돌이 일어난 정보를 담아서 건내주자.
	Collision2D collision2D;
	collision2D.collider = a;
	collision2D.otherCollider = b;
	collision2D.rigidbody = rbA;
	collision2D.otherRigidbody = rbB;
	collision2D.transform = a->GetTransform();

	// 이전 충돌정보와 비교해서 Enter인지 Stay인지 검증하자.
	if (CollisionSystem::GetInstance().previousCollisions.find(pair) == CollisionSystem::GetInstance().previousCollisions.end())
	{
		for (auto sc : scA) sc->OnCollisionEnter2D(&collision2D);
		for (auto sc : scB) sc->OnCollisionEnter2D(&collision2D);
	}
	else
	{
		for (auto sc : scA) sc->OnCollisionStay2D(&collision2D);
		for (auto sc : scB) sc->OnCollisionStay2D(&collision2D);
	}
}

bool Physics::FCollisionDetector::CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
    if (a->aabb.minVector.x < b->aabb.minVector.x)
    {
        return true;
    }
    else if (a->aabb.minVector.x > b->aabb.minVector.x)
    {
        return false;
    }
    else
    {
        return a->aabb.minVector.y < b->aabb.minVector.y;
    }
}

bool Physics::FCollisionDetector::IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
	return !(a.Get()->aabb.minVector.x > b.Get()->aabb.maxVector.x ||
		a.Get()->aabb.minVector.y > b.Get()->aabb.maxVector.y ||
		a.Get()->aabb.maxVector.x < b.Get()->aabb.minVector.x ||
		a.Get()->aabb.maxVector.y < b.Get()->aabb.minVector.y);
}

// 충돌을 감지하고 AABB 오버랩 된 부분을 밀어내는 로직입니다.
// 각 상태를 나누어 관리합니다. 
// 1. 땅 위
// 2. 땅 위에 있는 오브젝트의 위
// 3. 공기 중
//
// --- 이미 선언한 헬퍼 함수 그대로 사용 ---
// CalcXOverlap, CalcYOverlap, Width, IsOnGround

void Physics::FCollisionDetector::PushOverlappedArea(Collider* a, Collider* b)
{
	FAABB& aabb_a = a->aabb;
	FAABB& aabb_b = b->aabb;
	Rigidbody2D* rbA = a->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = b->GetOwner()->GetComponent<Rigidbody2D>();
	float massA = rbA ? rbA->mass : FLT_MAX;
	float massB = rbB ? rbB->mass : FLT_MAX;
	Define::ERigidBodyType typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
	Define::ERigidBodyType typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;

	// 겹친 거리 계산 (함수 사용)
	float overlap_x = CalcXOverlap(aabb_a, aabb_b);
	float overlap_y = CalcYOverlap(aabb_a, aabb_b);

	if (overlap_x <= Define::MIN_OVERLAP_EPSILON || overlap_y <= Define::MIN_OVERLAP_EPSILON)
		return;

	// 밀림 방향 결정
	float pushX = 0, pushY = 0;
	if (overlap_x < overlap_y)
	{
		pushX = (aabb_a.minVector.x < aabb_b.minVector.x) ? -overlap_x : overlap_x;
		pushY = 0;
	}
	else
	{
		pushY = (aabb_a.minVector.y < aabb_b.minVector.y) ? -overlap_y : overlap_y;
		pushX = 0;
	}

	constexpr float GROUND_OVERLAP_X_THRESHOLD = 0.3f;

	float totalMass = massA + massB;
	float ratioA = massA / totalMass;
	float ratioB = massB / totalMass;

	if (typeA == Define::ERigidBodyType::Dynamic && typeB == Define::ERigidBodyType::Dynamic)
	{
		// 중심점 계산
		float centerA_y = (aabb_a.minVector.y + aabb_a.maxVector.y) * 0.5f;
		float centerB_y = (aabb_b.minVector.y + aabb_b.maxVector.y) * 0.5f;

		Rigidbody2D* upper = nullptr;
		Rigidbody2D* lower = nullptr;

		// y축 기준 위/아래 판별
		if (centerA_y > centerB_y) {
			upper = rbA;
			lower = rbB;
		}
		else {
			upper = rbB;
			lower = rbA;
		}

		// upper(위에 있는 오브젝트) 상태 전이 (기존 그대로)
		switch (upper->m_eRigidBodyState)
		{
		case Define::ERigidBodyState::Space:
			switch (lower->m_eRigidBodyState)
			{
			case Define::ERigidBodyState::Space:
				break;
			case Define::ERigidBodyState::Ground:
			case Define::ERigidBodyState::OnRigidBody:
				upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
				break;
			}
			break;
		case Define::ERigidBodyState::Ground:
			break;
		case Define::ERigidBodyState::OnRigidBody:
			switch (lower->m_eRigidBodyState)
			{
			case Define::ERigidBodyState::Space:
				upper->m_eRigidBodyState = Define::ERigidBodyState::Space;
				break;
			case Define::ERigidBodyState::Ground:
				break;
			case Define::ERigidBodyState::OnRigidBody:
				break;
			}
			break;
		}

		// 둘 다 밀림 (질량 비율)
		if (rbA) rbA->collisionPush += FVector2(pushX * ratioA * 1.2f, pushY * ratioA);
		if (rbB) rbB->collisionPush += FVector2(-pushX * ratioB * 1.2f, -pushY * ratioB);
	}
	else if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic))
	{
		bool is_y_push = (pushY != 0);  // y축으로 밀림이 있을 때만
		bool is_on_ground = false;
		if (is_y_push && aabb_a.minVector.y <= aabb_b.maxVector.y) 
		{
			// 오로지 y축으로 아래(즉, 바닥에) 닿았고, x축 겹침 비율도 충분할 때만
			is_on_ground = IsOnGround(aabb_a, aabb_b, GROUND_OVERLAP_X_THRESHOLD);
		}
		if (rbA) rbA->m_eRigidBodyState = is_on_ground ? Define::ERigidBodyState::Ground : Define::ERigidBodyState::Space;
		if (rbA) rbA->collisionPush += FVector2(pushX * 1.5f, pushY);
	}
	else if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic)
	{
		bool is_y_push = (pushY != 0);
		bool is_on_ground = false;
		if (is_y_push && aabb_b.minVector.y <= aabb_a.maxVector.y) 
		{
			is_on_ground = IsOnGround(aabb_b, aabb_a, GROUND_OVERLAP_X_THRESHOLD);
		}
		if (rbB) rbB->m_eRigidBodyState = is_on_ground ? Define::ERigidBodyState::Ground : Define::ERigidBodyState::Space;
		if (rbB) rbB->collisionPush += FVector2(-pushX * 1.5f, -pushY);
	}
	// 나머지(Static-Static, Kinematic-Kinematic, Static-Kinematic): 아무것도 안 함
}


void Physics::FCollisionDetector::PushOverlappedAreaNoMass(Collider* a, Collider* b)
{
    // aabb 정보 가져오기
    const auto& aabb_a = a->aabb;
    const auto& aabb_b = b->aabb;

    // 겹친 거리 계산
    float overlap_x = min(aabb_a.maxVector.x, aabb_b.maxVector.x) - max(aabb_a.minVector.x, aabb_b.minVector.x);
    float overlap_y = min(aabb_a.maxVector.y, aabb_b.maxVector.y) - max(aabb_a.minVector.y, aabb_b.minVector.y);

    if (overlap_x < overlap_y)
    {
        float push = overlap_x / 2.0f;
        if (aabb_a.minVector.x < aabb_b.minVector.x)
        {
            a->GetOwner()->transform()->AddPosition(-push, 0);
            b->GetOwner()->transform()->AddPosition(push, 0);
        }
        else
        {
            a->GetOwner()->transform()->AddPosition(push, 0);
            b->GetOwner()->transform()->AddPosition(-push, 0);
        }
    }
    else
    {
        float push = overlap_y / 2.0f;
        if (aabb_a.minVector.y < aabb_b.minVector.y)
        {
            a->GetOwner()->transform()->AddPosition(0, -push);
            b->GetOwner()->transform()->AddPosition(0, push);
        }
        else
        {
            a->GetOwner()->transform()->AddPosition(0, push);
            b->GetOwner()->transform()->AddPosition(0, -push);
        }
    }
}

bool Physics::FCollisionDetector::LineAABBIntersect(const FVector2& p0, const FVector2& p1, const FAABB& aabb, FVector2& outHitPos)
{
    float tmin = 0.0f;
    float tmax = 1.0f;

    FVector2 d = p1 - p0;

    for (int axis = 0; axis < 2; ++axis)
    {
        float p = (axis == 0) ? d.x : d.y;
        float minVal = (axis == 0) ? aabb.minVector.x : aabb.minVector.y;
        float maxVal = (axis == 0) ? aabb.maxVector.x : aabb.maxVector.y;
        float p0val = (axis == 0) ? p0.x : p0.y;

        if (fabs(p) < 1e-6f) // 이동량이 거의 없는 경우
        {
            if (p0val < minVal || p0val > maxVal)
                return false; // 해당 축 범위를 벗어남
        }
        else
        {
            float t1 = (minVal - p0val) / p;
            float t2 = (maxVal - p0val) / p;

            if (t1 > t2) std::swap(t1, t2);
            tmin = max(tmin, t1);
            tmax = min(tmax, t2);
            if (tmin > tmax)
                return false;
        }
    }
    // 교차 지점 = p0 + (p1-p0) * tmin
    outHitPos = p0 + d * tmin;
    // (추가로 tmin이 0~1 구간 안에 있어야 함: 이미 위에서 보장)
    return true;
}

float Physics::FCollisionDetector::CalcXOverlap(const FAABB& a, const FAABB& b)
{
    return min(a.maxVector.x, b.maxVector.x) - max(a.minVector.x, b.minVector.x);
}

float Physics::FCollisionDetector::CalcYOverlap(const FAABB& a, const FAABB& b)
{
    return min(a.maxVector.y, b.maxVector.y) - max(a.minVector.y, b.minVector.y);
}

float Physics::FCollisionDetector::Width(const FAABB& box)
{
    return box.maxVector.x - box.minVector.x;
}

bool Physics::FCollisionDetector::IsOnGround(const FAABB& above, const FAABB& below, float threshold)
{
	float x_overlap = CalcXOverlap(above, below);
	float overlap_ratio_above = x_overlap / Width(above);
	float overlap_ratio_below = x_overlap / Width(below);
	return (overlap_ratio_above > threshold) || (overlap_ratio_below > threshold);
}
