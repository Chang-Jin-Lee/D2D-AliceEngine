#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>
#include <Define/Define.h>
#include <Experimental/Collision/CollisionDetector.h>
#include <Manager/TimerManager.h>

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
	m_prevPosition = m_currentPosition;
	m_currentPosition = owner->transform()->GetPosition();
	m_prevRotation = m_currentRotation = owner->transform()->GetRotation();
	attachedCollider = GetOwner() ? GetOwner()->GetComponent<Collider>() : nullptr;
}

void Rigidbody2D::Update(const float& deltaSeconds)
{
	if (auto col = GetOwner()->GetComponent<Collider>())
		if (col->dirty) return;

	if (!GetOwner()->transform()->bMoved)
		return;

	// FixedUpdate 이후 현재 위치 갱신
	m_prevPosition = m_currentPosition;
	m_prevRotation = m_currentRotation;

	const float dt = deltaSeconds;
	FVector2 calcPos = owner->transform()->GetPosition() + collisionPush;
	collisionPush = FVector2(0, 0);
	FVector2 calcVel = velocity;
	float calcAngle = owner->transform()->GetRotation();
	float calcAngularVel = angularVelocity;
	FVector2 calcForce = force;
	float calcTorque = torque;

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

		// x축 단축선 충돌 판정은 calcPos 갱신전에 수행!
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
		calcPos.x += (calcVel.x * dt) * minHitT;
		calcVel.x = 0.0f;
	}
	else
	{
		calcPos.x += calcVel.x * dt;
	}

	// 접지 처리
	if ((m_eRigidBodyState == Define::ERigidBodyState::Ground ||
		m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody) &&
		calcVel.y < 0)
	{
		calcVel.y = 0.0f;
	}

	// 최종 반영: 프레임 완료 후 현재 위치 갱신
	m_currentPosition = calcPos;
	m_currentRotation = calcAngle;

	owner->transform()->SetPosition(calcPos);
	owner->transform()->SetRotation(calcAngle);

	velocity = calcVel;
	angularVelocity = calcAngularVel;

	// 다음 (CollisionSystem 등) 업데이트 전 1회만 Update
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

FVector2 Rigidbody2D::GetPredictedPosition() const
{
	float alpha = TimerManager::GetInstance().GetAccumulator() / TimerManager::GetInstance().fixedDeltaTime;
	alpha = Math::clamp(alpha, 0.0f, 1.0f);

	// 속력을 반영한 예측 위치 값
	FVector2 predictedDelta = velocity * alpha * TimerManager::GetInstance().fixedDeltaTime;
	return m_currentPosition + predictedDelta;
}

FVector2 Rigidbody2D::GetInterpolatedPosition() const
{
	float alpha = TimerManager::GetInstance().GetAccumulator() / TimerManager::GetInstance().fixedDeltaTime;
	alpha = Math::clamp(alpha, 0.0f, 1.0f);

	// 선형 보간: prevPos + (currPos - prevPos) * alpha
	return FVector2(
		m_prevPosition.x + (m_currentPosition.x - m_prevPosition.x) * alpha,
		m_prevPosition.y + (m_currentPosition.y - m_prevPosition.y) * alpha
	);
}

float Rigidbody2D::GetInterpolatedRotation() const
{
	float alpha = TimerManager::GetInstance().GetAccumulator() / TimerManager::GetInstance().fixedDeltaTime;
	alpha = Math::clamp(alpha, 0.0f, 1.0f);

	return m_prevRotation + (m_currentRotation - m_prevRotation) * alpha;
}

void Rigidbody2D::UpdateInterpolatedPosition()
{
	FVector2 interpolatedPos = GetInterpolatedPosition();
	float interplatedRot = GetInterpolatedRotation();

	owner->transform()->SetPosition(interpolatedPos);
	owner->transform()->SetRotation(interplatedRot);
}