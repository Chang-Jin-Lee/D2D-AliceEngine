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
	if (Collider* col = GetOwner()->GetComponent<Collider>())
	{
		if (col->dirty) return;
	}
	if(GetOwner()->transform()->bMoved == false) return;
	float substep = 0.016f;
	float remaining = deltaSeconds;

	// 임시 포지션/속도/힘 값 (실제 transform이나 velocity에 바로 적용 X!)
	FVector2 calcPos = owner->transform()->GetPosition();
	calcPos += collisionPush;
	collisionPush = FVector2(0, 0);

	FVector2 calcVel = velocity;
	float calcAngle = owner->transform()->GetRotation();
	float calcAngularVel = angularVelocity;
	FVector2 calcForce = force;
	float calcTorque = torque;

	while (remaining > 0.0f)
	{
		float dt = min(substep, remaining);

		// 중력
		if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
		{
			FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
			calcForce += gravityForce;
		}

		FVector2 acceleration = calcForce / mass;
		calcVel += acceleration * dt;
		calcVel.x *= (1.0f - drag * dt);
		calcVel.y *= (1.0f - drag * dt);

		// y축 이동
		calcPos.y += calcVel.y * dt;
		calcAngle += calcAngularVel * dt;

		// x축 터널링 방지 알고리즘도 calcPos 기준으로!
		FVector2 prevPos = calcPos;
		FVector2 nextPos = prevPos + FVector2(calcVel.x * dt, 0);
		bool blockedX = false;
		float minHitT = 1.0f;
		FVector2 hitPos;

		for (auto& weakRb : CollisionSystem::GetInstance().m_colliders)
		{
			if (auto sp = weakRb.lock())
			{
				if (!sp || sp == owner->GetComponent<Collider>()) continue;
				if (Physics::FCollisionDetector::LineAABBIntersect(prevPos, nextPos, sp->aabb, hitPos))
				{
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
			float xMove = (calcVel.x * dt) * minHitT;
			calcPos.x += xMove;
			calcVel.x = 0.0f;
		}
		else
		{
			calcPos.x += calcVel.x * dt;
		}

		// 힘, 토크 초기화
		calcForce = FVector2(0.0f, 0.0f);
		calcTorque = 0.0f;

		// 7. 상태별 y속도 제한 (Ground, OnRigidBody에서만 아래로 가려는 힘 막음)
		if (m_eRigidBodyState == Define::ERigidBodyState::Ground ||
			m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody)
		{
			if (calcVel.y < 0)
				calcVel.y = 0.0f;
		}

		remaining -= dt;
	}

	// === while 끝나고 단 1회만 실제 적용 ===
	owner->transform()->SetPosition(calcPos);
	owner->transform()->SetRotation(calcAngle);
	velocity = calcVel;
	angularVelocity = calcAngularVel;

	// 이후 (CollisionSystem 등) 프레임당 1회만 Update
	force = FVector2(0.0f, 0.0f);
	torque = 0.0f;
}


void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x += _x * 1000;
	force.y += _y * 1000;
}