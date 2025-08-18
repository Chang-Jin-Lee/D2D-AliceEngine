#include "Sun.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Manager/SceneManager.h>
#include <System/ScriptSystem.h>
#include <Helpers/CoordHelper.h>

void Sun::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void Sun::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	m_sun->transform()->AddRotation(0.2f);
}

void Sun::OnStart()
{
	m_sun = GetOwner();
	m_sun->transform()->SetPivot(0.5f);
	m_sun->transform()->SetScale(0.5f);
	m_sun->AddComponent<TextRenderComponent>()->SetText(m_sun->GetName());
	m_sun->GetComponent<TextRenderComponent>()->SetDrawType(EDrawType::WorldSpace);
	m_sun->GetComponent<TextRenderComponent>()->SetRelativeScale(FVector2(2, 2));
	m_sun->AddComponent<SpriteRenderer>()->LoadData(L"Sun.png");
	m_sun->AddComponent<BoxComponent>(m_sun->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
	FVector2 buttonSize = m_sun->GetComponent<TextRenderComponent>()->GetRelativeSize();
	m_sun->GetComponent<TextRenderComponent>()->SetRelativePosition(CoordHelper::RatioCoordToScreen(buttonSize, FVector2(-0.5, -0.5)));

	m_sun->AddComponent<InputComponent>()->SetAction(m_sun->GetHandle(),[this]() { Input(); });
}

void Sun::OnEnd()
{
}

void Sun::Input()
{
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"FSMScene");
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if (m_sun->GetComponent<TextRenderComponent>()->GetText() == L"Sun")
			m_sun->GetComponent<TextRenderComponent>()->SetText(L"태양");
		else
			m_sun->GetComponent<TextRenderComponent>()->SetText(L"Sun");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		m_sun->transform()->AddRotation(5.0f);
	}
	if (Input::IsKeyDown(VK_C))
	{
		m_sun->transform()->AddRotation(-5.0f);
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_sun->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_sun->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		m_sun->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		m_sun->transform()->AddPosition(0, -5.0f);
	}
}
