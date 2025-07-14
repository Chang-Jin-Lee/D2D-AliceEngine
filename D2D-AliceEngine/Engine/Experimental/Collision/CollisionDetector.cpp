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
    
    std::unordered_set<Rigidbody2D*> ovelappedRigdBodies;
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
                if (Rigidbody2D* rb = src->GetOwner()->GetComponent<Rigidbody2D>()) ovelappedRigdBodies.insert(rb);
                if (Rigidbody2D* rb = tar->GetOwner()->GetComponent<Rigidbody2D>()) ovelappedRigdBodies.insert(rb);
                std::wcout << L"[�浹] " << src->GetName() << L" " << tar->GetName() << L" ��ħ\n";
            }
        }
    }
    return ovelappedRigdBodies;
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
	// aabb ���� ��������
	const auto& aabb_a = a->aabb;
	const auto& aabb_b = b->aabb;

	// Rigidbody2D ���
	Rigidbody2D* rbA = a->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = b->GetOwner()->GetComponent<Rigidbody2D>();
	if (rbA == nullptr || rbB == nullptr) return;

	// ���� ��� (������ ���Ѵ�� ����)
	float massA = rbA ? rbA->mass : FLT_MAX;
	float massB = rbB ? rbB->mass : FLT_MAX;
	float totalMass = massA + massB;

	// ��ģ �Ÿ� ���
	float overlap_x = min(aabb_a.maxVector.x, aabb_b.maxVector.x) - max(aabb_a.minVector.x, aabb_b.minVector.x);
	float overlap_y = min(aabb_a.maxVector.y, aabb_b.maxVector.y) - max(aabb_a.minVector.y, aabb_b.minVector.y);

	// �� ������Ʈ�� ������ ����
	float ratioA = (massB) / totalMass;
	float ratioB = (massA) / totalMass;

	const float pushMargin = 1.1f; // 10% �� �б�

	if (overlap_x < overlap_y)
	{
		float push = overlap_x * pushMargin; // ������ ����
		float moveA = -push * ratioA;
		float moveB = push * ratioB;

		if (aabb_a.minVector.x < aabb_b.minVector.x)
		{
			a->GetOwner()->transform()->AddPosition(moveA, 0);
			b->GetOwner()->transform()->AddPosition(moveB, 0);
		}
		else
		{
			a->GetOwner()->transform()->AddPosition(moveB, 0);
			b->GetOwner()->transform()->AddPosition(moveA, 0);
		}
	}
	else
	{
		float push = overlap_y * pushMargin; // ������ ����
		float moveA = -push * ratioA;
		float moveB = push * ratioB;

		if (aabb_a.minVector.y < aabb_b.minVector.y)
		{
			a->GetOwner()->transform()->AddPosition(0, moveA);
			b->GetOwner()->transform()->AddPosition(0, moveB);
		}
		else
		{
			a->GetOwner()->transform()->AddPosition(0, moveB);
			b->GetOwner()->transform()->AddPosition(0, moveA);
		}
	}

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
