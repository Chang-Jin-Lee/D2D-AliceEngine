#include "DemoScene.h"
#include <Component/InputComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Core/Input.h>
#include <Component/AnimationComponent.h>

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� weak_ptr�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene::Initialize()
{
}

void DemoScene::Release()
{
	__super::Release();
}

void DemoScene::Update()
{
	__super::Update();
	m_earth->transform()->AddRotation(0.5f); // ���� ����
	m_moon->transform()->AddRotation(2.0f); // �� ����
	m_sun->transform()->AddRotation(0.2f);
}

void DemoScene::OnEnter()
{
	__super::OnEnter();

	m_yuuka = NewObject<gameObject>(L"yuuka");
	m_yuuka->Initialize(FVector2(0, 0), 0.0f, FVector2(0.8f, 0.8f), FVector2(0.5f));
	m_yuuka->AddComponent<AnimationComponent>()->LoadData(L"BackGround\\Yuuka.mp4", 24);
	m_yuuka->GetComponent<AnimationComponent>()->Play();

	m_sun = NewObject<gameObject>(L"Sun");
	m_sun->Initialize(FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_sun->AddComponent<TextRenderComponent>()->SetText(m_sun->GetName());
	m_sun->GetComponent<TextRenderComponent>()->SetTransformType(ETransformType::Unity);
	m_sun->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_sun->GetComponent<TextRenderComponent>()->SetScale(FVector2(2, 2));
	m_sun->AddComponent<SpriteRenderer>()->LoadData(L"Sun.png");
	m_sun->AddComponent<BoxComponent>(m_sun->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);
	m_sun->GetComponent<TextRenderComponent>()->SetPosition(FVector2(0, -m_sun->GetComponent<SpriteRenderer>()->GetSize().y * 0.7f));

	m_earth = NewObject<gameObject>(L"Earth");
	m_earth->Initialize(FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_earth->AddComponent<SpriteRenderer>()->LoadData(L"Earth.png");
	m_earth->AddComponent<BoxComponent>(m_earth->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);
	m_earth->AddComponent<TextRenderComponent>()->SetText(m_earth->GetName());
	m_earth->GetComponent<TextRenderComponent>()->SetTransformType(ETransformType::Unity);
	m_earth->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_earth->GetComponent<TextRenderComponent>()->SetScale(FVector2(3, 3));
	m_earth->GetComponent<TextRenderComponent>()->SetPosition(FVector2(0, -m_earth->GetComponent<SpriteRenderer>()->GetSize().y * 0.7f));

	m_moon = NewObject<gameObject>(L"Moon");
	m_moon->Initialize(FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_moon->AddComponent<SpriteRenderer>()->LoadData(L"Moon.png");
	m_moon->AddComponent<BoxComponent>(m_moon->GetComponent<SpriteRenderer>()->GetSize(), FColor::Black);
	m_moon->AddComponent<TextRenderComponent>()->SetText(m_moon->GetName());
	m_moon->GetComponent<TextRenderComponent>()->SetTransformType(ETransformType::Unity);
	m_moon->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_moon->GetComponent<TextRenderComponent>()->SetScale(FVector2(4, 4));
	m_moon->GetComponent<TextRenderComponent>()->SetPosition(FVector2(0, -m_moon->GetComponent<SpriteRenderer>()->GetSize().y * 0.7f));

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->Initialize();
	m_widget->AddComponent<TextRenderComponent>()->SetText(L" <ī�޶�> \n [ȭ��ǥ ��,��] : ī�޶� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : ī�޶� ��,�� �̵� \n [1/2] : D2D, Unity ��ǥ�� \n\n <�¾�, ����, ��> \n [z,c] : �¾� ȸ�� \n [w,s] : �¾� ���� �̵� \n [a,d] : �¾� �¿� �̵� \n [4] : �¾� �̸� �ѿ� ��ȯ \n [b,m] : ���� ȸ�� \n [y,h] : ���� ���� �̵� \n [g,j] : ���� �¿� �̵� \n [9,0] : �� ȸ�� \n [o,l] : �� ���� �̵� \n [k.;] : �� �¿� �̵� \n\n");
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 150));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget2->Initialize();
	m_widget2->transform()->SetPosition(0,0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget3->Initialize();
	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_sun->AddChildObject(m_earth);
	m_earth->AddChildObject(m_moon);

	/*
	* ���ӿ�����Ʈ�� InputComponent�� ���̴� ����
	*/
	m_sun->AddComponent<InputComponent>()->SetAction([this]() { SunInput(); });
	m_earth->AddComponent<InputComponent>()->SetAction([this]() { EarthInput(); });
	m_moon->AddComponent<InputComponent>()->SetAction([this]() { MoonInput(); });
	m_sun->AddComponent<InputComponent>()->SetAction([this]() { CameraInput(); });
}

void DemoScene::OnExit()
{
	__super::OnExit();
}

void DemoScene::SunInput()
{
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if(m_sun->GetComponent<TextRenderComponent>()->GetText() == L"Sun")
			m_sun->GetComponent<TextRenderComponent>()->SetText(L"�¾�");
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
	if (Input::IsKeyDown(VK_D))
	{
		m_sun->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_A))
	{
		m_sun->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_W))
	{
		m_sun->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_S))
	{
		m_sun->transform()->AddPosition(0, -5.0f);
	}
}

void DemoScene::MoonInput()
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
	if (Input::IsKeyDown(VK_OEM_1)) // ���� ; Ű
	{
		m_moon->transform()->AddPosition(5.0f, 0);
	}
}

void DemoScene::EarthInput()
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


void DemoScene::CameraInput()
{
	if (Input::IsKeyDown(VK_RIGHT))
	{
		SceneManager::GetCamera()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		SceneManager::GetCamera()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		SceneManager::GetCamera()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		SceneManager::GetCamera()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyPressed(VK_1))
	{
		D2DRenderManager::Get().SwitchTransformType(ETransformType::D2D);
	}
	if (Input::IsKeyPressed(VK_2))
	{
		D2DRenderManager::Get().SwitchTransformType(ETransformType::Unity);
	}
}
