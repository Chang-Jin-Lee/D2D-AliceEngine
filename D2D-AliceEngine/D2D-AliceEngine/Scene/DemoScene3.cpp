#include "DemoScene3.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Aru.h"
#include "../Scripts/Aru2.h"
#include "../Scripts/Player.h"
#include "../Scripts/Enemy.h"

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� rawPointer�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene3::Initialize()
{
}

void DemoScene3::Release()
{
}

void DemoScene3::Update()
{
	__super::Update();
}

void DemoScene3::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");

	m_widget->AddComponent<TextRenderComponent>()->SetText(L" <�÷��̾�> \n [ȭ��ǥ ��,��] : �÷��̾� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : �÷��̾� ��,�� �̵� \n [z] : ���� \n\n\n [1/2] : D2D, Unity ��ǥ�� \n [Q] : ī�޶� �÷��̾�� ���̱� \n [E] : ī�޶� ���� \n\n");
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 50));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));

	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();

	//for (int i = 0; i < enemyMax; i++)
	//{
	//	m_enemies.push_back(NewObjectByWeak<gameObject>(L"enemy"));
	//	m_enemies[i].lock()->AddComponent<Enemy>();
	//	m_enemies[i].lock()->AddComponent<InputComponent>()->SetAction(m_enemies[i].lock()->GetHandle(), [this]() { EnemyInput(); });
	//}
	m_enemies.push_back(NewObject<gameObject>(L"enemy"));
	m_enemies[0]->AddComponent<Enemy>();
	m_enemies[0]->AddComponent<InputComponent>()->SetAction(m_enemies[0]->GetHandle(), [this]() { EnemyInput(); });

	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });
}

void DemoScene3::OnExit()
{
	__super::OnExit();
}

void DemoScene3::EnemyInput()
{
	if (Input::IsKeyPressed(VK_G))
	{
		m_player->GetComponent<Aru>()->m_aruStat->DecreaseAbility("HP", 5);
	}
}

void DemoScene3::PlayerInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_player);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}
	if (Input::IsKeyPressed(VK_T))
	{
		//m_enemy->GetComponent<Aru2>()->m_aru2Stat->DecreaseAbility("HP", 10);
		m_player->GetComponent<Aru>()->m_aruStat->DecreaseAbility("MP", 5);
	}
	if (Input::IsKeyPressed(VK_Y))
	{
		m_player->GetComponent<Aru>()->m_aruStat->IncreaseAbility("MAXHP", "HP", 5);
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_player->GetComponent<Aru>()->m_aruStat->SetStat("MAXHP", m_player->GetComponent<Aru>()->m_aruStat->GetStat("MAXHP") + 10);
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if (m_player->GetComponent<TextRenderComponent>()->GetText() == L"aru")
			m_player->GetComponent<TextRenderComponent>()->SetText(L"�Ʒ�");
		else
			m_player->GetComponent<TextRenderComponent>()->SetText(L"aru");
	}

	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
}