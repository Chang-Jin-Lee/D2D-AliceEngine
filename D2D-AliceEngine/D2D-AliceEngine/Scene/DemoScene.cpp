#include "DemoScene.h"
#include <Component/InputComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Core/Input.h>

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� weak_ptr�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene::Initialize()
{
}

void DemoScene::Release()
{
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

	m_sun = NewObject<gameObject>(L"Sun");
	m_sun->Initialize(FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_sun->AddComponent<SpriteRenderer>()->LoadData(L"Sun.png");
	m_sun->AddComponent<BoxComponent>(m_sun->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);

	m_earth = NewObject<gameObject>(L"Earth");
	m_earth->Initialize(FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_earth->AddComponent<SpriteRenderer>()->LoadData(L"Earth.png");
	m_earth->AddComponent<BoxComponent>(m_earth->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);

	m_moon = NewObject<gameObject>(L"Moon");
	m_moon->Initialize(FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_moon->AddComponent<SpriteRenderer>()->LoadData(L"Moon.png");
	m_moon->AddComponent<BoxComponent>(m_moon->GetComponent<SpriteRenderer>()->GetSize(), FColor::Black);

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->Initialize();
	m_widget->AddComponent<TextRenderComponent>()->SetContent(L" <ī�޶�> \n [ȭ��ǥ ��,��] : ī�޶� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : ī�޶� ��,�� �̵� \n [1/2] : D2D, Unity ��ǥ�� \n\n <�¾�, ����, ��> \n [z,c] : �¾� ȸ�� \n [w,s] : �¾� ���� �̵� \n [a,d] : �¾� �¿� �̵� \n [b,m] : ���� ȸ�� \n [y,h] : ���� ���� �̵� \n [g,j] : ���� �¿� �̵� \n [9,0] : �� ȸ�� \n [o,l] : �� ���� �̵� \n [k.;] : �� �¿� �̵� \n\n <�Ʒ�> \n [5,6] : ���� ����, ���� ���� \n [7,8] : ���� ����, ���� ���� \n [4,E,R,T] : �̵�");
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(0, 100));

	m_aru = NewObject<gameObject>(L"aru");
	m_aru->Initialize(FVector2(350, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_aru->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");

	m_sun->AddChildObject(m_earth);
	m_earth->AddChildObject(m_moon);

	m_aru->AddComponent<InputComponent>()->SetAction([this]() { aruInput(); });
	m_sun->AddComponent<InputComponent>()->SetAction([this]() { SunInput(); });
	m_earth->AddComponent<InputComponent>()->SetAction([this]() { EarthInput(); });
	m_moon->AddComponent<InputComponent>()->SetAction([this]() { MoonInput(); });
	m_sun->AddComponent<InputComponent>()->SetAction([this]() { CameraInput(); });
}

void DemoScene::OnExit()
{
	__super::OnExit();
}

void DemoScene::aruInput()
{
	if (Input::IsKeyPressed(VK_5))
	{
		gameObject* gun = NewObject<gameObject>(L"gun");
		gun->Initialize(FVector2(FRandom::GetRandomInRange(-512,512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		gun->AddComponent<SpriteRenderer>()->LoadData(L"gun.png");
	}
	if (Input::IsKeyPressed(VK_6))
	{
		RemoveObjectByName(L"gun");
	}
	if (Input::IsKeyPressed(VK_7))
	{
		gameObject* wallet = NewObject<gameObject>(L"wallet");
		wallet->Initialize(FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		wallet->AddComponent<SpriteRenderer>()->LoadData(L"wallet.png");
	}
	if (Input::IsKeyPressed(VK_8))
	{
		RemoveObjectByName(L"wallet");
	}
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::GetCamera()->SetOwner(m_aru);
	}

	if (GetAsyncKeyState(VK_T) & 0x8000)
	{
		m_aru->transform()->AddPosition(5.0f, 0);
	}
	if (GetAsyncKeyState(VK_E) & 0x8000)
	{
		m_aru->transform()->AddPosition(-5.0f, 0);
	}
	if (GetAsyncKeyState(VK_R) & 0x8000)
	{
		m_aru->transform()->AddPosition(0, -5.0f);
	}
	if (GetAsyncKeyState(VK_4) & 0x8000)
	{
		m_aru->transform()->AddPosition(0, 5.0f);
	}
}

void DemoScene::SunInput()
{
	if (GetAsyncKeyState(VK_Z) & 0x8000)
	{
		m_sun->transform()->AddRotation(5.0f);
	}
	if (GetAsyncKeyState(VK_C) & 0x8000)
	{
		m_sun->transform()->AddRotation(-5.0f);
	}
	if (GetAsyncKeyState(VK_D) & 0x8000)
	{
		m_sun->transform()->AddPosition(5.0f, 0);
	}
	if (GetAsyncKeyState(VK_A) & 0x8000)
	{
		m_sun->transform()->AddPosition(-5.0f, 0);
	}
	if (GetAsyncKeyState(VK_W) & 0x8000)
	{
		m_sun->transform()->AddPosition(0, -5.0f);
	}
	if (GetAsyncKeyState(VK_S) & 0x8000)
	{
		m_sun->transform()->AddPosition(0, 5.0f);
	}
}

void DemoScene::MoonInput()
{
	if (GetAsyncKeyState('9') & 0x8000)
	{
		m_moon->transform()->AddRotation(5.0f);
	}
	if (GetAsyncKeyState('0') & 0x8000)
	{
		m_moon->transform()->AddRotation(-5.0f);
	}
	if (GetAsyncKeyState(VK_O) & 0x8000)
	{
		m_moon->transform()->AddPosition(0, -5.0f);
	}
	if (GetAsyncKeyState(VK_L) & 0x8000)
	{
		m_moon->transform()->AddPosition(0, 5.0f);
	}
	if (GetAsyncKeyState(VK_K) & 0x8000)
	{
		m_moon->transform()->AddPosition(-5.0f, 0);
	}
	if (GetAsyncKeyState(VK_OEM_1) & 0x8000)
	{
		m_moon->transform()->AddPosition(5.0f, 0);
	}
}

void DemoScene::EarthInput()
{
	if (GetAsyncKeyState(VK_B) & 0x8000)
	{
		m_earth->transform()->AddRotation(5.0f);
	}
	if (GetAsyncKeyState(VK_M) & 0x8000)
	{
		m_earth->transform()->AddRotation(-5.0f);
	}
	if (GetAsyncKeyState(VK_J) & 0x8000)
	{
		m_earth->transform()->AddPosition(5.0f, 0);
	}
	if (GetAsyncKeyState(VK_G) & 0x8000)
	{
		m_earth->transform()->AddPosition(-5.0f, 0);
	}
	if (GetAsyncKeyState(VK_Y) & 0x8000)
	{
		m_earth->transform()->AddPosition(0, 5.0f);
	}
	if (GetAsyncKeyState(VK_H) & 0x8000)
	{
		m_earth->transform()->AddPosition(0, -5.0f);
	}
}

void DemoScene::CameraInput()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		SceneManager::GetCamera()->AddPosition(5.0f, 0);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		SceneManager::GetCamera()->AddPosition(-5.0f, 0);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		SceneManager::GetCamera()->AddPosition(0, 5.0f);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		SceneManager::GetCamera()->AddPosition(0, -5.0f);
	}
	if (GetAsyncKeyState(VK_1) & 0x8000)
	{
		D2DRenderManager::Get().SwitchTransformType(ETransformType::D2D);
	}
	if (GetAsyncKeyState(VK_2) & 0x8000)
	{
		D2DRenderManager::Get().SwitchTransformType(ETransformType::Unity);
	}
}
