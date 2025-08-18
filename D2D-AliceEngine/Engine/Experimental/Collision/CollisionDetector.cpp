#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/Rigidbody2D.h>
#include <Component/ScriptComponent.h>
#include <System/CollisionSystem.h>
#include <Component/BoxComponent.h>
#include "Collision2D.h"

static inline bool GetBoxWorldBounds(Collider* c, float& minX, float& maxX, float& minY, float& maxY)
{
	if (!c) return false;
	gameObject* go = c->GetOwner();
	if (!go) return false;
	FVector2 ownerPos = go->transform() ? go->transform()->GetPosition() : FVector2(0, 0);
	FVector2 size(0, 0);
	FVector2 center(0, 0);
	if (c->boxComponent && c->boxComponent->GetUsage() == BoxComponent::EUsage::Collision)
	{
		size = c->boxComponent->m_size;
		// 월드 중심은 BoxComponent의 상대 트랜스폼(월드 위치)을 우선 사용
		if (auto tr = c->boxComponent->GetRelativeTransform()) center = tr->GetPosition();
		else center = ownerPos + c->boxComponent->GetRelativePosition();
	}
	else
	{
		return false;
	}
	const float halfW = size.x * 0.5f;
	const float halfH = size.y * 0.5f;
	minX = center.x - halfW;
	maxX = center.x + halfW;
	minY = center.y - halfH;
	maxY = center.y + halfH;
	return true;
}

void Physics::FCollisionDetector::BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if(objects[i].expired()) continue;
		if (objects[i]->dirty) continue;
		for (size_t j = i + 1; j < objects.size(); ++j)
		{
			if (objects[j].expired()) continue;

			// 채널 필터링: 같은 채널만 검사
			if (objects[i]->GetCollisionChannel() != objects[j]->GetCollisionChannel()) continue;

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
	struct Entry
	{
		WeakObjectPtr<Collider> weak;
		float minX;
		float maxX;
	};

	std::vector<Entry> active;
	active.reserve(objects.size());

	// 살아있는 콜라이더만 스냅샷 생성
	for (auto& w : objects)
	{
		if (w.expired()) continue;
		if (CheckCollisionCondition(w)) continue;

		Collider* c = w.Get();
		float minX, maxX, minY, maxY;
		if (!GetBoxWorldBounds(c, minX, maxX, minY, maxY)) continue;
		active.push_back(Entry{ w, minX, maxX });
	}

	// minX로 정렬
	std::sort(active.begin(), active.end(),
		[](const Entry& a, const Entry& b) {
			return a.minX < b.minX;
		});

	std::unordered_set<Rigidbody2D*> overlappedRigidBodies;
	CollisionSystem::GetInstance().currentCollisions.clear();

	// Sweep & Prune
	for (size_t i = 0; i < active.size(); ++i)
	{
		auto& A = active[i];
		if (A.weak.expired()) continue;

		for (size_t j = i + 1; j < active.size(); ++j)
		{
			auto& B = active[j];
			if (B.weak.expired()) continue;

			// Prune
			if (B.maxX < A.minX)
				continue;

			// 실제 충돌 판정
			if (IsOverlapped(A.weak, B.weak))
			{
				Collider* ca = A.weak.Get();
				Collider* cb = B.weak.Get();
				if (!ca || !cb) continue;

				// 채널 필터링: 같은 채널만 이벤트/해결
				if (ca->GetCollisionChannel() != cb->GetCollisionChannel()) continue;

				PushOverlappedArea(ca, cb);
				SavePreviousCollisionData(ca, cb, overlappedRigidBodies);
			}
		}
	}

	LoadPreviousCollisions();
	return overlappedRigidBodies;
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
			collision2D.transform = a->GetOwnerTransform();

			std::vector<ScriptComponent*> scA, scB;
			if (a->GetOwner()) scA = a->GetOwner()->GetComponents<ScriptComponent>();
			if (b->GetOwner()) scB = b->GetOwner()->GetComponents<ScriptComponent>();
			if (collision2D.rigidbody && collision2D.rigidbody->m_eRigidBodyType == Define::ERigidBodyType::Dynamic && collision2D.otherRigidbody && collision2D.otherRigidbody->m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
			{
				for (auto sc : scA) if (a->GetLayer() == b->GetLayer()) sc->OnCollisionExit2D(&collision2D);
				for (auto sc : scB) if (a->GetLayer() == b->GetLayer()) sc->OnCollisionExit2D(&collision2D);
			}
			for (auto sc : scA) if (a->GetLayer() == b->GetLayer()) sc->OnTriggerExit2D(b);
			for (auto sc : scB) if (a->GetLayer() == b->GetLayer()) sc->OnTriggerExit2D(a);
		}
	}
	CollisionSystem::GetInstance().previousCollisions = CollisionSystem::GetInstance().currentCollisions;
}

void Physics::FCollisionDetector::SavePreviousCollisionData(Collider* src, Collider* tar, std::unordered_set<Rigidbody2D*>& overlappedRigdBodies)
{
	// 약한 핸들 스냅샷(중간 파괴 안전)
	WeakObjectPtr<Collider> wa = src ? src->WeakFromThis<Collider>() : WeakObjectPtr<Collider>{};
	WeakObjectPtr<Collider> wb = tar ? tar->WeakFromThis<Collider>() : WeakObjectPtr<Collider>{};

	// 리지드바디는 "현재 살아있으면" 즉시 셋에 추가 (파괴되면 다음 프레임에 자동 정리됨)
	if (auto a = wa.lock()) if (auto rb = a->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rb);
	if (auto b = wb.lock()) if (auto rb = b->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rb);

	// 충돌 페어는 Collider* 주소쌍을 쓰고 있었으므로, 현재 살아있을 때만 등록
	if (auto a = wa.lock())
		if (auto b = wb.lock())
		{
			auto pair = std::minmax(a, b);
			CollisionSystem::GetInstance().currentCollisions.insert(pair);

			// 스크립트들 약한 핸들로 스냅샷
			std::vector<WeakObjectPtr<ScriptComponent>> wScA, wScB;
			{
				auto list = a->GetOwner()->GetComponents<ScriptComponent>();
				wScA.reserve(list.size());
				for (auto* sc : list) if (sc) wScA.emplace_back(sc->WeakFromThis<ScriptComponent>());
			}
			{
				auto list = b->GetOwner()->GetComponents<ScriptComponent>();
				wScB.reserve(list.size());
				for (auto* sc : list) if (sc) wScB.emplace_back(sc->WeakFromThis<ScriptComponent>());
			}

			const bool isNew =
				(CollisionSystem::GetInstance().previousCollisions.find(pair) ==
					CollisionSystem::GetInstance().previousCollisions.end());

			auto safeLayerEq = [&]() -> bool {
				auto aa = wa.lock(); auto bb = wb.lock();
				return aa && bb && (aa->GetLayer() == bb->GetLayer());
				};

			auto callCollision = [&](auto memfn)
				{
					// 콜백 직전마다 생존 확인 + Collision2D 재구성(중간 파괴 안전)
					if (!safeLayerEq()) return;
					auto aa = wa.lock(); auto bb = wb.lock();
					if (!aa || !bb) return;

					Rigidbody2D* rbA = aa->GetOwner()->GetComponent<Rigidbody2D>();
					Rigidbody2D* rbB = bb->GetOwner()->GetComponent<Rigidbody2D>();

					Collision2D c{};
					c.collider = aa;
					c.otherCollider = bb;
					c.rigidbody = rbA;
					c.otherRigidbody = rbB;
					c.transform = aa->GetOwnerTransform();

					// A 쪽
					for (auto& wsc : wScA)
						if (auto sc = wsc.lock())
							if (safeLayerEq())
								(sc->*memfn)(&c);
					// B 쪽
					for (auto& wsc : wScB)
						if (auto sc = wsc.lock())
							if (safeLayerEq())
								(sc->*memfn)(&c);
				};

			auto callTrigger = [&](auto memfn)
				{
					if (!safeLayerEq()) return;
					auto aa = wa.lock(); auto bb = wb.lock();
					if (!aa || !bb) return;

					// A 쪽
					for (auto& wsc : wScA)
						if (auto sc = wsc.lock())
							if (safeLayerEq())
								(sc->*memfn)(bb);
					// B 쪽
					for (auto& wsc : wScB)
						if (auto sc = wsc.lock())
							if (safeLayerEq())
								(sc->*memfn)(aa);
				};

			const bool bothDynamic =
				(a->GetOwner()->GetComponent<Rigidbody2D>() &&
					b->GetOwner()->GetComponent<Rigidbody2D>() &&
					a->GetOwner()->GetComponent<Rigidbody2D>()->m_eRigidBodyType == Define::ERigidBodyType::Dynamic &&
					b->GetOwner()->GetComponent<Rigidbody2D>()->m_eRigidBodyType == Define::ERigidBodyType::Dynamic);

			if (isNew)
			{
				if (bothDynamic) {
					callCollision(&ScriptComponent::OnCollisionEnter2D);
				}
				callTrigger(&ScriptComponent::OnTriggerEnter2D);
			}
			else
			{
				if (bothDynamic) {
					callCollision(&ScriptComponent::OnCollisionStay2D);
				}
				callTrigger(&ScriptComponent::OnTriggerStay2D);
			}
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
	Collider* ca = a.Get();
	Collider* cb = b.Get();
	if (!ca || !cb) return false;
	float aminX, amaxX, aminY, amaxY;
	float bminX, bmaxX, bminY, bmaxY;
	if (!GetBoxWorldBounds(ca, aminX, amaxX, aminY, amaxY)) return false;
	if (!GetBoxWorldBounds(cb, bminX, bmaxX, bminY, bmaxY)) return false;
	return !(aminX > bmaxX || amaxX < bminX || aminY > bmaxY || amaxY < bminY);
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
	FAABB aabb_a; FAABB aabb_b;
	{
		float minX, maxX, minY, maxY;
		if (GetBoxWorldBounds(a, minX, maxX, minY, maxY)) { aabb_a.minVector.x = minX; aabb_a.maxVector.x = maxX; aabb_a.minVector.y = minY; aabb_a.maxVector.y = maxY; }
	}
	{
		float minX, maxX, minY, maxY;
		if (GetBoxWorldBounds(b, minX, maxX, minY, maxY)) { aabb_b.minVector.x = minX; aabb_b.maxVector.x = maxX; aabb_b.minVector.y = minY; aabb_b.maxVector.y = maxY; }
	}
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
		if (rbA) rbA->collisionPush = FVector2(pushX * 1.5f, pushY);
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
		if (rbB) rbB->collisionPush = FVector2(-pushX * 1.5f, -pushY);
	}
	// 나머지(Static-Static, Kinematic-Kinematic, Static-Kinematic): 아무것도 안 함
}


void Physics::FCollisionDetector::PushOverlappedAreaNoMass(Collider* a, Collider* b)
{
    // BoxComponent 기반 AABB 계산
    FAABB aabb_a; FAABB aabb_b;
    {
        float minX, maxX, minY, maxY;
        if (GetBoxWorldBounds(a, minX, maxX, minY, maxY)) { aabb_a.minVector.x = minX; aabb_a.maxVector.x = maxX; aabb_a.minVector.y = minY; aabb_a.maxVector.y = maxY; }
    }
    {
        float minX, maxX, minY, maxY;
        if (GetBoxWorldBounds(b, minX, maxX, minY, maxY)) { aabb_b.minVector.x = minX; aabb_b.maxVector.x = maxX; aabb_b.minVector.y = minY; aabb_b.maxVector.y = maxY; }
    }

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
