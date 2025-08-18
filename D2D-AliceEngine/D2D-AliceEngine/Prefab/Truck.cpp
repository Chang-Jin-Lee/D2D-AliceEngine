#include "Truck.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Component/Collider.h>
#include <Scene/Scene.h>

void Truck::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Truck::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
}

void Truck::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	m_owner->transform()->AddPosition(20 * deltaSeconds, 0);
	m_leftCollider->transform()->AddPosition(20 * deltaSeconds, 0);
	m_bottomCollider->transform()->AddPosition(20 * deltaSeconds, 0);
}

void Truck::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
}

void Truck::Awake()
{
}

void Truck::OnStart()
{
	truckPos = 200;

	m_owner = GetOwner();
	m_owner->transform()->SetPosition(truckPos, 0);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(0.3f);
	m_owner->transform()->SetPivot(0.5f);
	//m_owner->AddComponent<SpriteRenderer>()->LoadData(L"truck.png");

	float sizeX = m_owner->GetComponent<SpriteRenderer>()->GetBitmapSizeX() * m_owner->transform()->GetScale().x;
	float sizeY = m_owner->GetComponent<SpriteRenderer>()->GetBitmapSizeY() * m_owner->transform()->GetScale().y;

	m_leftCollider = GetWorld()->NewObject<gameObject>(L"LeftCollider");
	m_leftCollider->transform()->SetPosition(truckPos - sizeX / 2.0f, 0);
	m_leftCollider->transform()->SetRotation(0);
	m_leftCollider->transform()->SetPivot(0.5f);
	m_leftCollider->AddComponent<BoxComponent>();
	m_leftCollider->AddComponent<Collider>()->SetBoxSize(FVector2(10, sizeY));
	//m_leftCollider->AddComponent<Collider>()->SetBoxPosition(FVector2(20,0));

	m_bottomCollider = GetWorld()->NewObject<gameObject>(L"BottomCollider");
	m_bottomCollider->transform()->SetPosition(truckPos, -sizeY / 2.0f);
	m_bottomCollider->transform()->SetRotation(0);
	m_bottomCollider->transform()->SetPivot(0.5f);
	m_bottomCollider->AddComponent<BoxComponent>();
	m_bottomCollider->AddComponent<Collider>()->SetBoxSize(FVector2(sizeX, 10));
	//m_bottomCollider->AddComponent<Collider>()->SetBoxPosition(FVector2(0,-20));

	//m_owner->GetComponent<Collider>()->SetBoxPosition(FVector2(-20, 30));
}

void Truck::OnEnd()
{
}

void Truck::OnDestroy()
{
}

void Truck::OnCollisionEnter2D(Collision2D* collider)
{
}

void Truck::OnCollisionStay2D(Collision2D* collider)
{
}

void Truck::OnCollisionExit2D(Collision2D* collider)
{
}

void Truck::OnTriggerEnter2D(Collider* collider)
{
}

void Truck::OnTriggerStay2D(Collider* collider)
{
}

void Truck::OnTriggerExit2D(Collider* collider)
{
}
