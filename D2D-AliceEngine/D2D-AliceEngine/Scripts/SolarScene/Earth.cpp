#include "Earth.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Core/Input.h>
#include <Component/InputComponent.h>
#include <System/ScriptSystem.h>

void Earth::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);
}

void Earth::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	m_earth->transform()->AddRotation(0.5f); // 지구 자전
}

void Earth::OnStart()
{
	m_earth = GetOwner();
	m_earth->transform()->SetPosition(500, 0);
	m_earth->transform()->SetPivot(0.5f);
	m_earth->transform()->SetScale(0.5f);
	m_earth->AddComponent<SpriteRenderer>()->LoadData(L"Earth.png");
	m_earth->AddComponent<BoxComponent>(m_earth->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
	m_earth->AddComponent<TextRenderComponent>()->SetText(m_earth->GetName());
	m_earth->GetComponent<TextRenderComponent>()->SetDrawType(EDrawType::WorldSpace);
	//m_earth->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_earth->GetComponent<TextRenderComponent>()->SetRelativeScale(FVector2(3, 3));
	m_earth->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(0, -m_earth->GetComponent<SpriteRenderer>()->GetRelativeSize().y * 0.7f));

	m_earth->AddComponent<InputComponent>()->SetAction(m_earth->GetHandle(),[this]() { Input(); });
}

void Earth::OnEnd()
{
}

void Earth::Input()
{
	if (Input::IsKeyDown(VK_B))
	{
		m_earth->transform()->AddRotation(5.0f);
	}
	if (Input::IsKeyDown(VK_M))
	{
		m_earth->transform()->AddRotation(-5.0f);
	}
	if (Input::IsKeyDown(VK_J))
	{
		m_earth->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_G))
	{
		m_earth->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_Y))
	{
		m_earth->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_H))
	{
		m_earth->transform()->AddPosition(0, -5.0f);
	}
}
