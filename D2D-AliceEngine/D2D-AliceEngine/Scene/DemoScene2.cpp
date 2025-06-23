#include "DemoScene2.h"
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

void DemoScene2::Initialize()
{

}

void DemoScene2::Release()
{
}

void DemoScene2::Update()
{
	__super::Update();
}

void DemoScene2::OnEnter()
{
	__super::OnEnter();

	(void)StatTraits<MyStat>::GetOffsetMap();

	m_yuuka = NewObject<gameObject>(L"yuuka");
	m_yuuka->transform()->SetPosition(0, 0);
	m_yuuka->transform()->SetRotation(0);
	m_yuuka->transform()->SetScale(0.8f, 0.8f);
	m_yuuka->transform()->SetPivot(0.5f);
	m_yuuka->AddComponent<AnimationComponent>()->LoadData(L"BackGround\\YuukaBottle.mp4", 24);
	m_yuuka->GetComponent<AnimationComponent>()->Play();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(L" <ī�޶�> \n [ȭ��ǥ ��,��] : ī�޶� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : ī�޶� ��,�� �̵� \n [1/2] : D2D, Unity ��ǥ�� \n [Q] : ī�޶� �Ʒ翡�� ���̱� \n [E] : ī�޶� ���� \n * ī�޶� ���̸� ȭ��ǥ�� ī�޶� �̵��� �� �����ϴ�. \n\n <�Ʒ�> \n [5,6] : ���� ����, ���� �ı� \n [7,8] : ���� ����, ���� �ı� \n [W,A,S,D] : �̵� \n [4] : �Ʒ� �̸� �ѿ� ��ȯ");
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 150));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 139, 255));

	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_tree = NewObject<gameObject>(L"tree");
	m_tree->transform()->SetPosition(0, 0);
	m_tree->transform()->SetRotation(0);
	m_tree->transform()->SetScale(0.5f,0.5f);
	m_tree->transform()->SetPivot(0.5f);
	//m_tree->AddComponent<SpriteRenderer>()->LoadData(L"BackGround/BG_CS_Arona_04.png");

	m_aru = NewObject<gameObject>(L"aru");
	m_aru->transform()->SetPosition(0, 0);
	m_aru->transform()->SetRotation(0);
	m_aru->transform()->SetScale(0.5f, 0.5f);
	m_aru->transform()->SetPivot(0.5f);
	m_aru->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);

	/*
	* ���ӿ�����Ʈ�� TextRenderComponent�� ���̴� ����
	*/
	m_aruNameText = m_aru->AddComponent<TextRenderComponent>();
	m_aruNameText->SetText(m_aru->GetName());
	m_aruNameText->SetTransformType(ETransformType::Unity);
	m_aruNameText->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruNameText->SetScale(FVector2(3, 3));
	m_aruNameText->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.5f));

	m_aruStatText = m_aru->AddComponent<TextRenderComponent>();
	m_aruStatText->SetText(L"test");
	m_aruStatText->SetTransformType(ETransformType::Unity);
	m_aruStatText->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruStatText->SetScale(FVector2(3, 3));
	m_aruStatText->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.2f));

	m_aruStatText2 = m_aru->AddComponent<TextRenderComponent>();
	m_aruStatText2->SetText(L"test");
	m_aruStatText2->SetTransformType(ETransformType::Unity);
	m_aruStatText2->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruStatText2->SetScale(FVector2(3, 3));
	m_aruStatText2->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.3f));

	/*
	* Ŀ���� ����ü�� ��������Ʈ�� ���ε� �ϴ� ����
	*/
	m_aruStat = m_aru->AddComponent<StatComponent<MyStat>>();
	m_aruStat->SetStat("HP", 100);
	m_aruStatText->SetTextFormat( L"prev HP : ", m_aruStat->GetStat("HP"));
	m_aruStatText2->SetTextFormat(L"cur HP : ", m_aruStat->GetStat("HP"));

	m_aruStat->OnChangeStat.Add(nullptr, [this](const std::string& name, float oldVal, float newVal) 
	{
		if (newVal <= 0)
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);
		}
		m_aruStatText->SetTextFormat(L"prev HP : ", oldVal);
		m_aruStatText2->SetTextFormat(L"cur HP : ", newVal);
	});;

	/*
	* DefaultStat���� �⺻ ����ü�� ��������Ʈ�� ���ε��ϴ� ����. �������� ����. ���� ����ü�� ����⸦ ����.
	*/
	m_aruDefaultStat = m_aru->AddComponent<StatComponent<>>();


	m_aru->AddComponent<InputComponent>()->SetAction([this]() { aruInput(); });
	m_aru->AddComponent<InputComponent>()->SetAction([this]() { CameraInput(); });
}

void DemoScene2::OnExit()
{
	__super::OnExit();
}

void DemoScene2::aruInput()
{
	if (Input::IsKeyPressed(VK_T))
	{
		m_aruStat->TakeDamage("HP", 5);
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if (m_aru->GetComponent<TextRenderComponent>()->GetText() == L"aru")
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"�Ʒ�");
		else
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"aru");
	}
	if (Input::IsKeyPressed(VK_5))
	{
		gameObject* gun = NewObject<gameObject>(L"gun");
		gun->Initialize(FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
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
		SceneManager::ChangeScene(L"SolarSystemScene");
	}
	if (Input::IsKeyDown(VK_D))
	{
		m_aru->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_A))
	{
		m_aru->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_S))
	{
		m_aru->transform()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyDown(VK_W))
	{
		m_aru->transform()->AddPosition(0, 5.0f);
	}
}

void DemoScene2::CameraInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_aru);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}
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
