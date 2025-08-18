#include "Aru.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/StatComponent.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Helpers/CoordHelper.h>
#include <Manager/TimerManager.h>
#include "Aru2.h"

void Aru::Initialize()
{
	__super::Initialize();
	(void)StatTraits<AruStat>::GetOffsetMap();
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}


void Aru::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Aru::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
	// 
	// 마우스 클릭 감지

	if (Input::IsMouseLeftDown() && bCanFire)
	{
		FVector2 ownerPos = owner->GetPosition();
		FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
		//BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, 300, EDroneType::Player);
		bCanFire = false;
	}
}

void Aru::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Aru::Invoke()
{

}

void Aru::OnStart()
{
	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bCanFire = true;
			//OutputDebugStringW(L"람다 타이머 호출됨!\n");
		}, 
		0.1f,
		true,
		0.0f
	);

	// 여기에 OnStart에 대한 로직 작성
	m_aru = GetOwner();
	m_aru->transform()->SetPosition(0, 0);
	m_aru->transform()->SetRotation(0);
	m_aru->transform()->SetScale(0.25f, 0.25f);
	m_aru->AddComponent<SpriteRenderer>()->LoadData(L"DemoGame1/aru.png");
	m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetRelativeSize(), FColor::Blue);
	m_aru->GetComponent<BoxComponent>()->SetSize(m_aru->GetComponent<SpriteRenderer>()->GetRelativeSize());
	m_aru->GetComponent<BoxComponent>()->SetIgnoreOwnerScale(false);

	/*
	* 게임오브젝트에 TextRenderComponent를 붙이는 예시
	*/

	TextRenderComponent* m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(m_aru->GetName());
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetFontSize(24);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	FVector2 widgetSize = m_aruTextCmp->GetRelativeSize();
	m_aruTextCmp->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 5.2f)));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);


	/*
	* 커스텀 구조체로 델리게이트를 바인딩 하는 예제
	*/
	m_aruStat = m_aru->AddComponent<StatComponent<AruStat>>();
	m_aruStat->SetStat("HP", 30);
	m_aruStat->SetStat("MAXHP", 30);
	m_aruStat->SetStat("MP", 200);
	m_aruNameTexts[1]->SetTextFormat(L"직전 체력 : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[2]->SetTextFormat(L"현재 체력 : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[3]->SetTextFormat(L"최대 체력 : ", m_aruStat->GetStat("MAXHP"));
	m_aruNameTexts[4]->SetTextFormat(L"마나 : ", m_aruStat->GetStat("MP"));

	widgetSize = m_aruNameTexts[1]->GetRelativeSize();
	m_aruNameTexts[1]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 1.0)));
	widgetSize = m_aruNameTexts[2]->GetRelativeSize();
	m_aruNameTexts[2]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 2.0f)));
	widgetSize = m_aruNameTexts[3]->GetRelativeSize();
	m_aruNameTexts[3]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 3.0f)));
	widgetSize = m_aruNameTexts[4]->GetRelativeSize();
	m_aruNameTexts[4]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 0.0f)));

	m_aruStat->OnChangeStatMap["HP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		if (newVal <= 0)	// 죽는 시점
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"DemoGame1/dead.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
		}
		else if (oldVal <= 0)	// 부활하는 시점
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"DemoGame1/aru.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
		}
		m_aruNameTexts[1]->SetTextFormat(L"직전 체력 : ", oldVal);
		m_aruNameTexts[2]->SetTextFormat(L"현재 체력 : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MAXHP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aruNameTexts[3]->SetTextFormat(L"최대 체력 : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aruNameTexts[4]->SetTextFormat(L"마나 : ", newVal);
	});;

	m_aru->AddComponent<InputComponent>()->SetAction(m_aru->GetHandle(), [this]() { Input(); });
}

void Aru::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	TimerManager::GetInstance().ClearTimer(timer);
}

void Aru::OnDestroy()
{
	m_aruNameTexts.clear();
}

void Aru::Input()
{
	// 여기에 Input에 대한 로직 작성

	if (Input::IsKeyPressed(VK_4))
	{
		if (m_aru->GetComponent<TextRenderComponent>()->GetText() == L"aru")
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"아루");
		else
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"aru");
	}
	if (Input::IsKeyPressed(VK_9))
	{
		//for (int i = 0; i < 5000; i++)
		//{
		//	GetWorld()->RemoveObjectByName(L"gun");
		//}
		GetWorld()->RemoveAllObjectsByName(L"gun");
	}
	if (Input::IsKeyPressed(VK_0))
	{
		for (int i = 0; i < 5000; i++)
		{
			gameObject* gun = GetWorld()->NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.1f, 0.5f), FRandom::GetRandomInRange(0.1f, 0.5f)), FVector2(0.5f));
			gun->AddComponent<SpriteRenderer>()->LoadData(L"DemoGame1/Gun.png");
		}
	}
	if (Input::IsKeyDown(VK_5))
	{
		// 생성자로 넣을 수도 있음
		gameObject* gun = GetWorld()->NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		gun->SetPosition(FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)));
		//gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
		gun->AddComponent<Aru2>();

		// 주석된 예시는 transform을 하나씩 set하는 방법
		//gameObject* gun = NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		//gun->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512));
		//gun->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//gun->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//gun->transform()->SetPivot(0.5f);
		//gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
	}
	if (Input::IsKeyDown(VK_6))
	{
		GetWorld()->RemoveObjectByName(L"gun");
	}
	if (Input::IsKeyDown(VK_7))
	{
		gameObject* wallet = GetWorld()->NewObject<gameObject>(L"wallet", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		wallet->AddComponent<SpriteRenderer>()->LoadData(L"DemoGame1/wallet.png");
		//wallet->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512));
		//wallet->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//wallet->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//wallet->transform()->SetPivot(0.5f);
	}
	if (Input::IsKeyDown(VK_8))
	{
		GetWorld()->RemoveObjectByName(L"wallet");
	}
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"SolarSystemScene");
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_aru->GetComponent<SpriteRenderer>()->SetFlip(true);
		m_aru->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_aru->GetComponent<SpriteRenderer>()->SetFlip(false);
		m_aru->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		m_aru->transform()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		m_aru->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_K))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.width += 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.x += 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.height += 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.y += 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_O))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.width -= 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.x -= 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_P))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.height -= 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.y -= 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
}

void Aru::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void Aru::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void Aru::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}
