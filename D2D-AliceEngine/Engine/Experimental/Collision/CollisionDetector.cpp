#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/Rigidbody2D.h>

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
				std::wcout << L"[�浹] " << objects[i]->GetName()<< " " << objects[j]->GetName() << " ��ħ\n";
			}
		}
	}
}

std::unordered_set<Rigidbody2D*> Physics::FCollisionDetector::SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
    // Sweep and Prune�� ���� ����
    std::sort(objects.begin(), objects.end(), &FCollisionDetector::CompareColliderMinX);
    
    std::unordered_set<Rigidbody2D*> overlappedRigdBodies;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->dirty) continue;
        auto& src = objects[i];

        for (size_t j = i + 1; j < objects.size(); ++j)
        {
            if (objects[j]->dirty) continue;
            auto& tar = objects[j];
            // Prune: �� �̻� ��ĥ �� ������ break
            if (tar->aabb.minVector.x > src->aabb.maxVector.x)
                break;

            // ���� �浹 �˻�
            if (IsOverlapped(src, tar))
            {
                PushOverlappedArea(src.Get(), tar.Get());
				if (Rigidbody2D* rbA = src->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rbA);
				if (Rigidbody2D* rbB = tar->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rbB);

                /*auto typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
                auto typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;*/

                //// Dynamic-Static(�Ǵ� Kinematic) �浹�� set�� �߰�
                //if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic))
                //    ovelappedRigdBodies.insert(rbA);
                //if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic)
                //    ovelappedRigdBodies.insert(rbB);
                //// Dynamic-Dynamic������ ���� ����!
            }
        }
    }
    return overlappedRigdBodies;
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

// �浹�� �����ϰ� AABB ������ �� �κ��� �о�� �����Դϴ�.
// �� ���¸� ������ �����մϴ�. 
// 1. �� ��
// 2. �� ���� �ִ� ������Ʈ�� ��
// 3. ���� ��
//
void Physics::FCollisionDetector::PushOverlappedArea(Collider* a, Collider* b)
{
	FAABB& aabb_a = a->aabb;
	FAABB& aabb_b = b->aabb;

	Rigidbody2D* rbA = a->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = b->GetOwner()->GetComponent<Rigidbody2D>();

	auto massA = rbA ? rbA->mass : FLT_MAX;
	auto massB = rbB ? rbB->mass : FLT_MAX;
	auto typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
	auto typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;

	// ��ģ �Ÿ� ���
	float overlap_x = min(aabb_a.maxVector.x, aabb_b.maxVector.x) - max(aabb_a.minVector.x, aabb_b.minVector.x);
    float overlap_y = min(aabb_a.maxVector.y, aabb_b.maxVector.y) - max(aabb_a.minVector.y, aabb_b.minVector.y);

	// �и� ���� ����
	float pushX = 0, pushY = 0;
	if (overlap_x < overlap_y) 
    {
		pushX = (aabb_a.minVector.x < aabb_b.minVector.x) ? -overlap_x : overlap_x;
	}
    else 
    {
		pushY = (aabb_a.minVector.y < aabb_b.minVector.y) ? -overlap_y : overlap_y;
	}

    float weight = 1.1f;
    pushX *= weight;
    pushY *= weight;

	float totalMass = massA + massB;
	float ratioA = massA / totalMass;
	float ratioB = massB / totalMass;

	if (typeA == Define::ERigidBodyType::Dynamic && typeB == Define::ERigidBodyType::Dynamic)
    {
        // �߽��� ���
        float centerA_y = (aabb_a.minVector.y + aabb_a.maxVector.y) * 0.5f;
        float centerB_y = (aabb_b.minVector.y + aabb_b.maxVector.y) * 0.5f;

        Rigidbody2D* upper = nullptr;
        Rigidbody2D* lower = nullptr;

        // y�� ���� ��/�Ʒ� �Ǻ�
        if (centerA_y > centerB_y) {
            upper = rbA;
            lower = rbB;
        }
        else {
            upper = rbB;
            lower = rbA;
        }

        // upper(���� �ִ� ������Ʈ) ���� ����
        // y�� �����̴ϱ� ���� �ִ� ������Ʈ, �Ʒ��� �ִ� ������Ʈ�� ������.
        // ������ ���� �ִ� ������Ʈ�� �������� ���¸� ������.
        switch (upper->m_eRigidBodyState)
        {
        case Define::ERigidBodyState::Space:
            switch (lower->m_eRigidBodyState)
            {
            case Define::ERigidBodyState::Space:
                // �� �� ����: ��ȭ ����
                break;
            case Define::ERigidBodyState::Ground:
                upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
                break;
            case Define::ERigidBodyState::OnRigidBody:
                upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
                break;
            }
            break;
        case Define::ERigidBodyState::Ground:
            // ���� �ִ� �ְ� Ground�� ���� ���� ����(Ư����Ȳ)
            break;
        case Define::ERigidBodyState::OnRigidBody:
            switch (lower->m_eRigidBodyState)
            {
            case Define::ERigidBodyState::Space:
                upper->m_eRigidBodyState = Define::ERigidBodyState::Space;
                break;
            case Define::ERigidBodyState::Ground:
                // �Ʒ��� Ground�� ���� OnRigidBody ����
                break;
            case Define::ERigidBodyState::OnRigidBody:
                break;
            }
            break;
        }

        // �� �� �и� (���� ����)
        a->GetOwner()->transform()->AddPosition(pushX * ratioA, pushY * ratioA);
        b->GetOwner()->transform()->AddPosition(-pushX * ratioB, -pushY * ratioB);
	}
	else if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic)) 
    {
		// A�� �и�
		if (aabb_a.minVector.y <= aabb_b.maxVector.y)
        {
            //rbA->isGrounded = true;
            // ��Ȯ�ϰ� ���� ��������, �̰� x���� ���� �ε�ġ�� ���.
            if(!(aabb_a.minVector.x > aabb_b.maxVector.x - 20 || aabb_a.maxVector.x < aabb_b.minVector.x + 20))
                rbA->m_eRigidBodyState = Define::ERigidBodyState::Ground;
        }
		else
		{
            rbA->m_eRigidBodyState = Define::ERigidBodyState::Space;
			//rbA->isGrounded = false;
		}
		a->GetOwner()->transform()->AddPosition(pushX * 1.6f, pushY);
	}
	else if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic) 
    {
		// B�� �и�
		if (aabb_b.minVector.y <= aabb_a.maxVector.y)
		{
            if (!(aabb_b.minVector.x > aabb_a.maxVector.x - 20 || aabb_b.maxVector.x < aabb_a.minVector.x + 20))
                rbB->m_eRigidBodyState = Define::ERigidBodyState::Ground;
		}
		else
		{
            rbB->m_eRigidBodyState = Define::ERigidBodyState::Space;
		}
		b->GetOwner()->transform()->AddPosition(-pushX * 1.6f, -pushY);
	}
	// ������(Static-Static, Kinematic-Kinematic, Static-Kinematic): �ƹ��͵� �� ��
}

void Physics::FCollisionDetector::PushOverlappedAreaNoMass(Collider* a, Collider* b)
{
    // aabb ���� ��������
    const auto& aabb_a = a->aabb;
    const auto& aabb_b = b->aabb;

    // ��ģ �Ÿ� ���
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

        if (fabs(p) < 1e-6f) // �̵����� ���� ���� ���
        {
            if (p0val < minVal || p0val > maxVal)
                return false; // �ش� �� ������ ���
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
    // ���� ���� = p0 + (p1-p0) * tmin
    outHitPos = p0 + d * tmin;
    // (�߰��� tmin�� 0~1 ���� �ȿ� �־�� ��: �̹� ������ ����)
    return true;
}
