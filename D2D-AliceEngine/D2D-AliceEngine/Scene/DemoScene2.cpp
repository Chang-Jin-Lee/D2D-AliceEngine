#include "DemoScene2.h"
#include <Component/InputComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Core/Input.h>
#include <Math/TColor.h>

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
	m_yuuka->transform()->SetScale(1.0f, 1.0f);
	m_yuuka->transform()->SetPivot(0.5f);
	m_yuuka->AddComponent<AnimationComponent>()->LoadData(L"BackGround\\YuukaBottle.mp4", 30);
	m_yuuka->GetComponent<AnimationComponent>()->Play();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(L" <ī�޶�> \n [ȭ��ǥ ��,��] : ī�޶� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : ī�޶� ��,�� �̵� \n [1/2] : D2D, Unity ��ǥ�� \n [Q] : ī�޶� �Ʒ翡�� ���̱� \n [E] : ī�޶� ���� \n * ī�޶� ���̸� ȭ��ǥ�� ī�޶� �̵��� �� �����ϴ�. \n\n <��� ����> \n [P] : ��� ����, ���� \n\n <�Ʒ�> \n [5,6] : ���� ����, ���� �ı� \n [7,8] : ���� ����, ���� �ı� \n [W,A,S,D] : �̵� \n [4] : �Ʒ� �̸� �ѿ� ��ȯ \n\n ::Delegate \n [T] �Ʒ簡 �Ʒ�2���� ������ �ֱ� \n [Y] : �Ʒ� 5 ȸ���ϱ� \n [U] : �Ʒ� �ִ�ü�� 10 �ø��� \n\n [G] �Ʒ�2�� �Ʒ翡�� ������ �ֱ� \n [H] : �Ʒ�2 7 ȸ���ϱ� \n [J] : �Ʒ� �ִ�ü�� 15 �ø���");
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 70));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));

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
	m_aru->transform()->SetScale(0.3f, 0.3f);
	m_aru->transform()->SetPivot(0.5f);
	m_aru->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);

	m_aru2 = NewObject<gameObject>(L"aru2");
	m_aru2->transform()->SetPosition(300, 0);
	m_aru2->transform()->SetRotation(0);
	m_aru2->transform()->SetScale(0.5f, 0.5f);
	m_aru2->transform()->SetPivot(0.5f);
	m_aru2->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);

	/*
	* ���ӿ�����Ʈ�� TextRenderComponent�� ���̴� ����
	*/
	TextRenderComponent* m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(m_aru->GetName());
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.5f));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.2f));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.3f));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.4f));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru->GetComponent<SpriteRenderer>()->GetSize().y * 0.1f));
	m_aruNameTexts.push_back(m_aruTextCmp);

	// ===========================
	m_aruTextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(m_aru2->GetName());
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetSize().y * 0.5f));
	m_aru2NameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetSize().y * 0.2f));
	m_aru2NameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetSize().y * 0.3f));
	m_aru2NameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetSize().y * 0.4f));
	m_aru2NameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetTransformType(ETransformType::Unity);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetScale(FVector2(3, 3));
	m_aruTextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetSize().y * 0.1f));
	m_aru2NameTexts.push_back(m_aruTextCmp);

	/*
	* Ŀ���� ����ü�� ��������Ʈ�� ���ε� �ϴ� ����
	*/
	m_aruStat = m_aru->AddComponent<StatComponent<MyStat>>();
	m_aruStat->SetStat("HP", 30);
	m_aruStat->SetStat("MAXHP", 30);
	m_aruStat->SetStat("MP", 200);
	m_aruNameTexts[1]->SetTextFormat(L"���� ü�� : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[2]->SetTextFormat(L"���� ü�� : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", m_aruStat->GetStat("MAXHP"));
	m_aruNameTexts[4]->SetTextFormat(L"���� : ", m_aruStat->GetStat("MP"));

	m_aruStat->OnChangeStatMap["HP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		if (newVal <= 0)	// �״� ����
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);
		}
		else if (oldVal <= 0)	// ��Ȱ�ϴ� ����
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);
		}
		m_aruNameTexts[1]->SetTextFormat(L"���� ü�� : ", oldVal);
		m_aruNameTexts[2]->SetTextFormat(L"���� ü�� : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MAXHP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aruNameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
		{
			m_aruNameTexts[4]->SetTextFormat(L"���� : ", newVal);
		});;


	m_aru2Stat = m_aru2->AddComponent<StatComponent<MyStat>>();
	m_aru2Stat->SetStat("HP", 100);
	m_aru2Stat->SetStat("MAXHP", 100);
	m_aru2Stat->SetStat("MP", 150);
	m_aru2NameTexts[1]->SetTextFormat(L"���� ü�� : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[2]->SetTextFormat(L"���� ü�� : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", m_aru2Stat->GetStat("MAXHP"));
	m_aru2NameTexts[4]->SetTextFormat(L"���� : ", m_aru2Stat->GetStat("MP"));

	m_aru2Stat->OnChangeStatMap["HP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
		{
			if (newVal <= 0)	// �״� ����
			{
				m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
				m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
				m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);
			}
			else if (oldVal <= 0)	// ��Ȱ�ϴ� ����
			{
				m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
				m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
				m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetSize(), FColor::Blue);
			}

			m_aru2NameTexts[1]->SetTextFormat(L"���� ü�� : ", oldVal);
			m_aru2NameTexts[2]->SetTextFormat(L"���� ü�� : ", newVal);
		});;

	m_aru2Stat->OnChangeStatMap["MAXHP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
		{
			m_aru2NameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", newVal);
		});;

	m_aru2Stat->OnChangeStatMap["MP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
		{
			m_aru2NameTexts[4]->SetTextFormat(L"���� : ", newVal);
		});;


	/*
	* DefaultStat���� �⺻ ����ü�� ��������Ʈ�� ���ε��ϴ� ����. �������� ����. ���� ����ü�� ����⸦ ����.
	*/
	m_aruDefaultStat = m_aru->AddComponent<StatComponent<>>();

	m_yuuka->AddComponent<InputComponent>()->SetAction([this]() { yuukaInput(); });
	m_aru->AddComponent<InputComponent>()->SetAction([this]() { aruInput(); });
	m_aru2->AddComponent<InputComponent>()->SetAction([this]() { aru2Input(); });
	m_aru->AddComponent<InputComponent>()->SetAction([this]() { CameraInput(); });
}

void DemoScene2::OnExit()
{
	__super::OnExit();
	m_aruNameTexts.clear();
	m_aru2NameTexts.clear();
}

void DemoScene2::yuukaInput()
{
	if (Input::IsKeyPressed(VK_P))
	{
		if (m_yuuka->GetComponent<AnimationComponent>()->bPlay == true)
			m_yuuka->GetComponent<AnimationComponent>()->Stop();
		else
			m_yuuka->GetComponent<AnimationComponent>()->Play();
	}
}

void DemoScene2::aru2Input()
{
	if (Input::IsKeyPressed(VK_G))
	{
		m_aruStat->DecreaseAbility("HP", 5);
		m_aru2Stat->DecreaseAbility("MP", 4);
	}
	if (Input::IsKeyPressed(VK_H))
	{
		m_aru2Stat->IncreaseAbility("MAXHP", "HP", 7);
	}
	if (Input::IsKeyPressed(VK_J))
	{
		m_aru2Stat->SetStat("MAXHP", m_aru2Stat->GetStat("MAXHP") + 15);
	}
}

void DemoScene2::aruInput()
{
	if (Input::IsKeyPressed(VK_T))
	{
		m_aruStat->DecreaseAbility("MP", 5);
		m_aru2Stat->DecreaseAbility("HP", 10);
	}
	if (Input::IsKeyPressed(VK_Y))
	{
		m_aruStat->IncreaseAbility("MAXHP", "HP", 5);
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_aruStat->SetStat("MAXHP", m_aruStat->GetStat("MAXHP")+10);
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if (m_aru->GetComponent<TextRenderComponent>()->GetText() == L"aru")
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"�Ʒ�");
		else
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"aru");
	}
	if (Input::IsKeyDown(VK_5))
	{
		//for (int i = 0; i < 2; i++)
		//{
		//	gameObject* gun = NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		//	gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
		//}

		// �ּ��� ���ô� transform�� �ϳ��� set�ϴ� ���
		// �����ڷ� ���� ���� ����
		gameObject* gun = NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
		//gameObject* gun = NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		//gun->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512));
		//gun->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//gun->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//gun->transform()->SetPivot(0.5f);
		//gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
	}
	if (Input::IsKeyDown(VK_6))
	{
		RemoveObjectByName(L"gun");
	}
	if (Input::IsKeyDown(VK_7))
	{
		gameObject* wallet = NewObject<gameObject>(L"wallet");
		wallet->Initialize(FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		//wallet->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512));
		//wallet->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//wallet->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//wallet->transform()->SetPivot(0.5f);
		wallet->AddComponent<SpriteRenderer>()->LoadData(L"wallet.png");
	}
	if (Input::IsKeyDown(VK_8))
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
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::D2D);
	}
	if (Input::IsKeyPressed(VK_2))
	{
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::Unity);
	}
}
