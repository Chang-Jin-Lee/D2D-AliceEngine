#include "Moon.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <System/ScriptSystem.h>

void Moon::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void Moon::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	m_moon->transform()->AddRotation(2.0f); // 달 자전
}

void Moon::OnStart()
{
	m_moon = GetOwner();
	m_moon->transform()->SetPosition(300, 0);
	m_moon->transform()->SetPivot(0.5f);
	m_moon->transform()->SetScale(0.5f);
	m_moon->AddComponent<SpriteRenderer>()->LoadData(L"Moon.png");
	m_moon->AddComponent<BoxComponent>(m_moon->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Black);
	m_moon->AddComponent<TextRenderComponent>()->SetText(m_moon->GetName());
	m_moon->GetComponent<TextRenderComponent>()->SetDrawType(EDrawType::WorldSpace);
	//m_moon->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_moon->GetComponent<TextRenderComponent>()->SetRelativeScale(FVector2(4, 4));
	m_moon->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(0, -m_moon->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.7f));

	m_moon->AddComponent<InputComponent>()->SetAction(m_moon->GetHandle(),[this]() { Input(); });
}

void Moon::OnEnd()
{

}

void Moon::Input()
{
	if (Input::IsKeyDown('9'))
	{
		m_moon->transform()->AddRotation(5.0f);
	}
	if (Input::IsKeyDown('0'))
	{
		m_moon->transform()->AddRotation(-5.0f);
	}
	if (Input::IsKeyDown(VK_O))
	{
		m_moon->transform()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_moon->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_K))
	{
		m_moon->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_OEM_1)) // 보통 ; 키
	{
		m_moon->transform()->AddPosition(5.0f, 0);
	}
}
