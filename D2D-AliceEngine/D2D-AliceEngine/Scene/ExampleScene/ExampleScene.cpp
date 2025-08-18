#include "ExampleScene.h"
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
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

// 이 밑은 게임 콘텐츠에서 정의한 스크립트 컴포넌트들입니다.
#include "../../Scripts/BackGroundImage.h"
#include "../../Scripts/Legacy/Aru.h"
#include "../../Scripts/Legacy/Aru2.h"
#include "../../Scripts/Legacy/Player.h"
#include "../../Scripts/Legacy/Enemy.h"

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*
*   Update() 함수에 if문으로 인풋을 넣을 수 도 있습니다. 혹은 Enter() 에서 InputComponent에 SetAction으로 인풋을 등록할 수 있습니다.
* 
*	player->GetComponent<Rigidbody2D>()->m_eRigidBodyState 처럼 컴포넌트에 있는 변수 사용할때 nullptr 체크 꼭 할것.
*	if(player->GetComponent<Rigidbody2D>()) player->GetComponent<Rigidbody2D>()->m_eRigidBodyState;
*/

void ExampleScene::Initialize()
{
	__super::Initialize();
}

void ExampleScene::Release()
{
	__super::Release();
}

void ExampleScene::Update()
{
	__super::Update();
	// player의 position, rotation, scale을 가져오는 방법
	FVector2 pos = m_player->transform()->GetPosition();
	float rotation = m_player->transform()->GetRotation();
	FVector2 scale = m_player->transform()->GetScale();

	// for문으로 돌면서 관리하는 방법
	// 땅에 있는면 Collider의 색을 초록색으로, 아니면 빨간색으로 바꾸는 로직
	for (int i = 0; i < enemyMax; i++)
	{
		if (m_enemies[i]->GetComponent<Rigidbody2D>())
		{
			if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
			{
				if (m_enemies[i]->GetComponent<Collider>())
					m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Green);
			}
			else
			{
				if (m_enemies[i]->GetComponent<Collider>())
					m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Red);
			}
		}
		
	}
	if (m_player->GetComponent<Rigidbody2D>())
	{
		if (m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
		{
			if (m_player->GetComponent<Collider>())
				m_player->GetComponent<Collider>()->SetBoxColor(FColor::Green);
		}
		else
		{
			if (m_player->GetComponent<Collider>())
				m_player->GetComponent<Collider>()->SetBoxColor(FColor::Red);
		}
	}
}

void ExampleScene::OnEnter()
{
	__super::OnEnter();

	// 위젯을 생성하고 컴포넌트를 부착하는 예시입니다.
	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <플레이어> \n"
		L" 정보를 넣으면 됩니다.\n"
		L" 텍스트 텍스트 \n"
		L" 텍스트 텍스트 텍스트 \n"
		L"\n"
	);
	m_widget->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(18, 60));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));
	m_widget->GetComponent<TextRenderComponent>()->SetLayer(20);

	// 백그라운드 배경 입니다.
	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	// 플레이어 예시입니다.
	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });


	// 적 예시입니다. 적은 vector에 담아 처리합니다.
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
		m_enemies[i]->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.9f));
		m_enemies[i]->transform()->SetPivot(0.5f);

		m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(FRandom::GetRandomInRange(10, 50), FRandom::GetRandomInRange(30, 100)));
		m_enemies[i]->AddComponent<Rigidbody2D>();
		m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
		m_enemies[i]->GetComponent<Rigidbody2D>()->gravityScale = 6;
		m_enemies[i]->GetComponent<Rigidbody2D>()->mass = 10;
	}

	// 벽 예시입니다.
	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));
}

void ExampleScene::OnExit()
{
	__super::OnExit();
	m_enemies.clear();
}

void ExampleScene::EnemyInput()
{
}

void ExampleScene::PlayerInput()
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
		if(m_player->GetComponent<Player>())
			m_player->GetComponent<Player>()->walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		if(m_player->GetComponent<Player>())
			m_player->GetComponent<Player>()->walkSpeed -= 5.0f;
	}
	if (Input::IsKeyDown(VK_G))
	{
		if(m_player->GetComponent<Rigidbody2D>())
			m_player->GetComponent<Rigidbody2D>()->gravityScale += 0.1f;
	}
	if (Input::IsKeyDown(VK_H))
	{
		if(m_player->GetComponent<Rigidbody2D>())
			m_player->GetComponent<Rigidbody2D>()->gravityScale -= 0.1f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		if(m_player->GetComponent<Player>())
			m_player->GetComponent<Player>()->bMoveRigidBody = !m_player->GetComponent<Player>()->bMoveRigidBody;
	}
}
