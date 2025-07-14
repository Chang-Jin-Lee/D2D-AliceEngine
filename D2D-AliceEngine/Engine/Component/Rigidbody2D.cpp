#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>

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
	//FVector2 pos = owner->transform()->GetPosition();
	// 1. �߷� ���� (isGrounded�� �ƴϸ�)
	if (!isGrounded && m_eRigidBodyType != Define::ERigidBodyType::Static)
	{
		// F = m * a, �߷°��ӵ��� �뷫 9.8, ���⼱ -12�� ����
		FVector2 gravityForce(0, -9.8f * gravityScale * mass);
		force += gravityForce;
	}

	// 2. ���ӵ� ��� (a = F/m)
	FVector2 acceleration = force / mass;

	// 3. �ӵ� ������Ʈ (v = v0 + at)
	velocity += acceleration * deltaSeconds;

	// 4. ���� ����
	velocity -= velocity * drag * deltaSeconds;
	angularVelocity -= angularVelocity * angularDrag * deltaSeconds;

	// 5. ��ġ, ȸ�� ������Ʈ
	owner->transform()->AddPosition(velocity.x * deltaSeconds, velocity.y * deltaSeconds);
	owner->transform()->AddRotation(angularVelocity * deltaSeconds);

	// 6. ��/��ũ �ʱ�ȭ (�����Ӹ���)
	force = FVector2(0.0f, 0.0f);
	torque = 0.0f;

	// 7. �浹 ó��: ���� ������� y�ӵ� 0, isGrounded true
	if (isGrounded)
	{
		velocity.y = 0.0f;
	}
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x = _x;
	force.y = _y;
}