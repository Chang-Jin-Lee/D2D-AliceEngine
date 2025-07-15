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
    return a->aabb.minVector.x < b->aabb.minVector.x;
}

bool Physics::FCollisionDetector::IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
	return !(a.Get()->aabb.minVector.x > b.Get()->aabb.maxVector.x ||
		a.Get()->aabb.minVector.y > b.Get()->aabb.maxVector.y ||
		a.Get()->aabb.maxVector.x < b.Get()->aabb.minVector.x ||
		a.Get()->aabb.maxVector.y < b.Get()->aabb.minVector.y);
}

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

	float totalMass = massA + massB;
	float ratioA = massA / totalMass;
	float ratioB = massB / totalMass;

    float weight = 1.05f;
	pushX *= weight;
	pushY *= weight;

	if (typeA == Define::ERigidBodyType::Dynamic && typeB == Define::ERigidBodyType::Dynamic)
    {
		// �� �� �и� (���� ����)
		a->GetOwner()->transform()->AddPosition(pushX * ratioA, pushY * ratioA);
		b->GetOwner()->transform()->AddPosition(-pushX * ratioB, -pushY * ratioB);
	}
	else if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic)) 
    {
		// A�� �и�
		a->GetOwner()->transform()->AddPosition(pushX, pushY);
        if (a->GetOwner()->transform()->GetPosition().y >= aabb_b.maxVector.y)
        {
            rbA->isGrounded = true;
        }
        else
        {
            rbA->isGrounded = false;
        }
	}
	else if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic) 
    {
		// B�� �и�
		b->GetOwner()->transform()->AddPosition(-pushX, -pushY);
		if (b->GetOwner()->transform()->GetPosition().y >= aabb_a.maxVector.y)
		{
			rbB->isGrounded = true;
		}
		else
		{
			rbA->isGrounded = false;
		}
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
