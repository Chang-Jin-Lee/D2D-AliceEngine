#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>
#include <Define/Define.h>
#include <Experimental/Collision/CollisionDetector.h>

Rigidbody2D::Rigidbody2D()
{

}

Rigidbody2D::~Rigidbody2D()
{
	PhysicsSystem::GetInstance().UnRegist(WeakFromThis<Rigidbody2D>());
}

void Rigidbody2D::Initialize()
{
	PhysicsSystem::GetInstance().Regist(WeakFromThis<Rigidbody2D>());
}

void Rigidbody2D::Update(const float& deltaSeconds)
{
	float substep = 0.016f;
	float remaining = deltaSeconds;
	while (remaining > 0.0f)
	{
		float dt = min(substep, remaining);

		// 1. �߷� �� �߰�
		if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
		{
			FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
			force += gravityForce;
		}

		// 2. ��ü ���ӵ� ���
		FVector2 acceleration = force / mass;

		// 3. �ӵ� ����
		velocity += acceleration * dt;

		// 4. ����(����) ����
		velocity.x *= (1.0f - drag * dt);
		velocity.y *= (1.0f - drag * dt);

		// 5. ��ġ ����
		owner->transform()->AddPosition(velocity.x * dt, velocity.y * dt);
		owner->transform()->AddRotation(angularVelocity * dt);


		// ========== x�� �ͳθ� ���� ���� ==========
		// �̵� �� ���� ��ġ
		FVector2 prevPos = owner->transform()->GetPosition();
		// �̵� ���� ��ġ (x��)
		FVector2 nextPos = prevPos + FVector2(velocity.x * dt, 0);

		bool blockedX = false;
		float minHitT = 1.0f; // 0~1, �浹 Ÿ�̹�
		FVector2 hitPos;

		// (�� �� ��� Collider ������� sweep)
		for (auto& weakRb : CollisionSystem::GetInstance().m_colliders)
		{
			if (auto sp = weakRb.lock())
			{
				if (!sp || sp == owner->GetComponent<Collider>()) continue;
				// ����-�ڽ� ���� �˻� (���⼭�� �ܼ�ȭ ����, AABB Sweep�� �� ���� ����)
				if (Physics::FCollisionDetector::LineAABBIntersect(prevPos, nextPos, sp->aabb, hitPos))
				{
					// (t = prevPos ~ nextPos�� ����, 0~1)
					float dx = nextPos.x - prevPos.x;
					float t = dx != 0 ? (hitPos.x - prevPos.x) / dx : 1.0f;
					if (t < minHitT)
					{
						minHitT = t;
						blockedX = true;
					}
				}
			}
		}

		if (blockedX)
		{
			// �浹 ��ġ���� �̵� (t ������ŭ)
			float xMove = (velocity.x * dt) * minHitT;
			owner->transform()->AddPosition(xMove, 0);
			velocity.x = 0.0f; // ����
		}
		else
		{
			owner->transform()->AddPosition(velocity.x * dt, 0);
		}

		// ========== x�� �ͳθ� ���� �� ==========

		// 6. �� �ʱ�ȭ
		force = FVector2(0.0f, 0.0f);
		torque = 0.0f;

		// 7. ���º� y�ӵ� ���� (Ground, OnRigidBody������ �Ʒ��� ������ �� ����)
		if (m_eRigidBodyState == Define::ERigidBodyState::Ground ||
			m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody)
		{
			if (velocity.y < 0)
				velocity.y = 0.0f;
		}

		remaining -= dt;
	}
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x += _x * 1000;
	force.y += _y * 1000;
}