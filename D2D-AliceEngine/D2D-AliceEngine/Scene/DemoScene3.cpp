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
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene3::Initialize()
{
	__super::Initialize();
}

void DemoScene3::Release()
{
	__super::Release();
}

void DemoScene3::Update()
{
	__super::Update();
	auto rigidType = m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyType;
	std::wstring rigidTypeStr = L"";
	switch (rigidType)
	{
	case Define::ERigidBodyType::Static:
		rigidTypeStr = L"Static";
		break;
	case Define::ERigidBodyType::Kinematic:
		rigidTypeStr = L"Kinematic";
		break;
	case Define::ERigidBodyType::Dynamic:
		rigidTypeStr = L"Dynamic";
		break;
	default:
		rigidTypeStr = L"Unknown";
		break;
	}


	FVector2 pos = m_player->transform()->GetPosition();
	float rotation = m_player->transform()->GetRotation();
	FVector2 scale = m_player->transform()->GetScale();
	m_widget4->GetComponent<TextRenderComponent>()->SetText(
		L" [info] \n"
		L" player Position : ("
		+ std::to_wstring(pos.x)
		+ L", "
		+ std::to_wstring(pos.y)
		+ L")"
		L"\n"
		L" player Rotation : ("
		+ std::to_wstring(rotation)
		+ L")"
		L"\n"
		L" player Scale : ("
		+ std::to_wstring(scale.x)
		+ L", "
		+ std::to_wstring(scale.y)
		+ L")"
		L"\n"
		L" player movement : ("
		+ (m_player->GetComponent<Player>()->bMoveRigidBody ? L"RigidBody AddForce" : L"Transform SetTransform")
		+ L")"
		L"\n"
		L" player Speed : ("
		+ std::to_wstring(m_player->GetComponent<Player>()->walkSpeed)
		+ L")"
		L"\n"
		L" player Gravity : ( -9.8 * "
		+ std::to_wstring(m_player->GetComponent<Rigidbody2D>()->gravityScale)
		+ L")"
		L"\n"
		L" Rigidbody Type : "
		+ rigidTypeStr
	);
	for (int i = 0; i < enemyMax; i++)
	{
		//m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(50, 80));
		if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
		{
			m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Green);
		}
		else if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody)
		{
			m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Blue);
		}
		else
		{
			m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Red);
		}
	}
	if (m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
	{
		m_player->GetComponent<Collider>()->SetBoxColor(FColor::Green);
	}
	else
	{
		m_player->GetComponent<Collider>()->SetBoxColor(FColor::Red);
	}
}

void DemoScene3::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget4 = NewObject<gameObject>(L"widget4");

	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <플레이어> \n"
		L" [화살표 상,하] : 플레이어 위,아래 이동\n"
		L" [화살표 좌/우] : 플레이어 좌,우 이동 \n"
		L" [z] : 공격 \n"
		L" [SpaceBar] : 점프 (2단 점프까지 가능) \n"
		L" [k] : 플레이어 속도 증가 - 터널링 검사 \n"
		L" [l] : 플레이어 속도 감소\n"
		L" [g] : 플레이어 중력 증가 - 터널링 검사 \n"
		L" [h] : 플레이어 중력 감소\n"
		L" [u] : 플레이어 이동 방식 변경\n"
		L" [i] : Rigidbody를 Kinematic으로 전환\n"
		L" [o] : Rigidbody를 Dynamic으로 전환\n"
		L" [p] : Rigidbody를 Static으로 전환\n"
		L"\n"
		L" <플레이어 상태>\n"
		L" Box 색상 : 초록색 - 땅 위에 있음\n"
		L" Box 색상 : 파란색/보라색 - 땅 위에 있는 물체 위에 있음\n"
		L" Box 색상 : 빨간색 - 공기 중에 있음\n"
		L" Rigidbody 타입에 따라 물리 시뮬레이션 적용/미적용이 달라집니다.\n"
		L" - Static : 움직이지 않음(지형/벽 등)\n"
		L" - Kinematic : 직접 제어, 물리 힘 무시\n"
		L" - Dynamic : 중력/힘 등 물리 적용\n"
		L" [1/2] : D2D, Unity 좌표계 \n"
		L" [Q] : 카메라를 플레이어에게 붙이기 \n"
		L" [E] : 카메라를 떼기 \n"
		L" [R] : Scene 재시작 \n"
		L"\n"
	);	
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(18, 60));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));
	m_widget->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <씬> \n [3] : 씬 전환");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget2->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <현재 씬> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget3->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget4->transform()->SetPosition(512, 512);
	m_widget4->AddComponent<TextRenderComponent>()->SetText(L" test");
	m_widget4->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget4->GetComponent<TextRenderComponent>()->SetPosition(FVector2(45, 60));
	m_widget4->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget4->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });

	for (int i = 0; i < enemyMax; i++)
	{
		m_enemies.push_back(NewObject<gameObject>(L"enemy"));
		m_enemies[i]->AddComponent<Enemy>();
		m_enemies[i]->AddComponent<InputComponent>()->SetAction(m_enemies[i]->GetHandle(), [this]() { EnemyInput(); });

		// 플레이어보다 조금 떨어진 위치에서 생성되게하는 로직
		/*m_enemies[i]->transform()->SetPosition(
			FRandom::GetRandomPointInTorus2D(
				m_player->transform()->GetPosition().x,
				m_player->transform()->GetPosition().y,
				200.0f,
				412.0f));*/
				// 특정 지점에서 원 모양이내에 랜덤판 위치에서 생성되게하는 로직
		m_enemies[i]->transform()->SetPosition(
			FRandom::GetRandomPointInCircle2D(
				300,
				300,
				200.0f
			));

		m_enemies[i]->transform()->SetRotation(0);
		m_enemies[i]->transform()->SetScale(FRandom::GetRandomInRange(0.3, 0.9f));
		m_enemies[i]->transform()->SetPivot(0.5f);

		//m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(50, 80));
		m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(FRandom::GetRandomInRange(10, 50), FRandom::GetRandomInRange(30, 100)));
		m_enemies[i]->AddComponent<Rigidbody2D>();
		m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
		m_enemies[i]->GetComponent<Rigidbody2D>()->gravityScale = 6;
		m_enemies[i]->GetComponent<Rigidbody2D>()->mass = 10;
	}

	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));

	m_wall2 = NewObject<gameObject>(L"wall2");
	m_wall2->transform()->SetPivot(0.5f);
	m_wall2->transform()->SetPosition(-400, 0);
	m_wall2->AddComponent<Collider>()->SetBoxSize(FVector2(60, 5500));

	m_wall2 = NewObject<gameObject>(L"wall2");
	m_wall2->transform()->SetPivot(0.5f);
	m_wall2->transform()->SetPosition(700, 0);
	m_wall2->AddComponent<Collider>()->SetBoxSize(FVector2(60, 5500));
}

void DemoScene3::OnExit()
{
	__super::OnExit();
	m_enemies.clear();
}

void DemoScene3::EnemyInput()
{
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
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
	if (Input::IsKeyPressed(VK_R))
	{
		SceneManager::RestartScene();
	}
	if (Input::IsKeyDown(VK_K))
	{
		m_player->GetComponent<Player>()->walkSpeed += 5;
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_player->GetComponent<Player>()->walkSpeed -= 5;
	}
	if (Input::IsKeyDown(VK_G))
	{
		m_player->GetComponent<Rigidbody2D>()->gravityScale += 0.1;
	}
	if (Input::IsKeyDown(VK_H))
	{
		m_player->GetComponent<Rigidbody2D>()->gravityScale -= 0.1;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_player->GetComponent<Player>()->bMoveRigidBody = !m_player->GetComponent<Player>()->bMoveRigidBody;
	}
	if (Input::IsKeyPressed(VK_I))
	{
		m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
	}
	if (Input::IsKeyPressed(VK_O))
	{
		m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
	}
	if (Input::IsKeyPressed(VK_P))
	{
		m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Static;
	}
}