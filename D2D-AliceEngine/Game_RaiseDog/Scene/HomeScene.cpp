#include "HomeScene.h"
#include <Component/TextRenderComponent.h>
#include <Manager/SceneManager.h>
#include <Core/Input.h>
#include <Object/gameObject.h>
#include <Component/StatComponent.h>
#include <Scripts/Widget/DogStatusWidgetScript.h>
#include <Component/ButtonComponent.h>
#include <Helper/ParticleHelper.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/CoordHelper.h>

static const wchar_t* GradeFromValue(float v)
{
	if (v >= 120.f) return L"U";
	if (v >= 110.f) return L"SS";
	if (v >= 100.f) return L"S";
	if (v >= 90.f) return L"A";
	if (v >= 80.f) return L"B";
	if (v >= 70.f) return L"C";
	if (v >= 60.f) return L"D";
	return L"E";
}

void HomeScene::Initialize()
{
	__super::Initialize();
}

void HomeScene::Release()
{
	__super::Release();
}

void HomeScene::Update()
{
	__super::Update();

	// 빈 공간 클릭 이펙트 (버튼 위가 아닐 때는 Scene 기본에서 처리한다고 가정)
	if (Input::IsMouseLeftPressed())
	{
		// 이 씬 로컬에서도 연출 추가 가능
	}
}

void HomeScene::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget_home");
	auto* title = m_widget->AddComponent<TextRenderComponent>();
	title->SetText(L" <집>  왕쨩 더비 - 스테이터스");
	title->SetRelativePosition(FVector2(40, 40));
	title->SetFontSize(26.0f);

	// Dog object with stats
	m_dog = NewObject<gameObject>(L"dog");
	auto dogStat = m_dog->AddComponent<StatComponent<DogStat>>();
	// Optional: initialize sample values
	dogStat->SetStat("HP", 88.f);
	dogStat->SetStat("STAMINA", 75.f);
	dogStat->SetStat("SPEED", 102.f);
	dogStat->SetStat("POWER", 95.f);
	dogStat->SetStat("GUTS", 83.f);
	dogStat->SetStat("WISDOM", 72.f);
	dogStat->SetStat("CHARM", 120.f);
	dogStat->SetStat("FRIENDLINESS", 110.f);

	// UI widget
	m_dogWidget = NewObject<gameObject>(L"dog_status_widget");
	m_dogWidget->SetPosition({160,160});
	auto widget = m_dogWidget->AddComponent<DogStatusWidgetScript>();
	widget->SetDogStat(dogStat);

	FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
	FVector2 center(app.x * 0.5f, app.y * 0.5f);

	// =================== 버튼 위치를 한 번에 제어할 베이스 좌표 ===================
	float btnBaseOffsetX = 0.0f;   // 필요시 X 이동
	float btnBaseOffsetY = 20.0f;  // 필요시 Y 이동 (메인 기준 +20)
	FVector2 btnBaseCenter(center.x + btnBaseOffsetX, center.y + btnBaseOffsetY);
	const float menuDeltaY = 80.0f;

	// 버튼 배치: 공원으로, 메인메뉴로 (중앙)
	{
		auto parkBtnObj = NewObject<gameObject>(L"btn_to_park");
		auto* parkBtn = parkBtnObj->AddComponent<ButtonComponent>();
		parkBtn->LoadData(Define::EButtonState::Idle, L"UI/Btn_Park_Idle.png");
		parkBtn->LoadData(Define::EButtonState::Hover, L"UI/Btn_Park_Hover.png");
		parkBtn->LoadData(Define::EButtonState::Pressed, L"UI/Btn_Park_Pressed.png");
		parkBtn->SetFallbackSize(FVector2(260, 70));
		parkBtn->SetFallbackColor(FColor(80, 255, 170, 255));
		FVector2 parkSize = parkBtn->GetRelativeSize();
		FVector2 parkCenter(btnBaseCenter.x, btnBaseCenter.y);
		parkBtn->SetRelativePosition(CoordHelper::RatioCoordToScreen(parkSize, FVector2(0.f, 0.f)) + parkCenter);
		parkBtn->SetStateAction(Define::EButtonState::Release, []() {
			ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
			SceneManager::ChangeScene(L"Park");
		});

		// 버튼 라벨을 동일 오브젝트에 추가하고 버튼 컴포넌트의 자식으로 연결
		auto* parkText = parkBtnObj->AddComponent<TextRenderComponent>();
		parkText->SetText(L"공원으로");
		parkText->SetFontSize(24.0f);
		parkText->SetColor(FColor(255,255,255,255));
		parkText->SetTextAlignment(ETextFormat::MiddleCenter);
		FVector2 parkTextSize = parkText->GetRelativeSize();
		parkText->SetRelativePosition(
			CoordHelper::RatioCoordToScreen(parkTextSize, FVector2(-0.5f, -0.5f))
		);
		parkBtn->RemoveFromParent();
		parkBtn->AddChildComponent(parkText);

		auto menuBtnObj = NewObject<gameObject>(L"btn_to_menu");
		auto* menuBtn = menuBtnObj->AddComponent<ButtonComponent>();
		menuBtn->LoadData(Define::EButtonState::Idle, L"UI/Btn_Menu_Idle.png");
		menuBtn->LoadData(Define::EButtonState::Hover, L"UI/Btn_Menu_Hover.png");
		menuBtn->LoadData(Define::EButtonState::Pressed, L"UI/Btn_Menu_Pressed.png");
		menuBtn->SetFallbackSize(FVector2(260, 70));
		menuBtn->SetFallbackColor(FColor(255, 200, 120, 255));
		FVector2 menuSize = menuBtn->GetRelativeSize();
		FVector2 menuCenter(btnBaseCenter.x, btnBaseCenter.y + menuDeltaY);
		menuBtn->SetRelativePosition(CoordHelper::RatioCoordToScreen(menuSize, FVector2(0.f, 0.f)) + menuCenter);
		menuBtn->SetStateAction(Define::EButtonState::Release, []() {
			ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
			SceneManager::ChangeScene(L"MainMenu");
		});

		auto* menuText = menuBtnObj->AddComponent<TextRenderComponent>();
		menuText->SetText(L"메인 메뉴");
		menuText->SetFontSize(24.0f);
		menuText->SetColor(FColor(255,255,255,255));
		menuText->SetTextAlignment(ETextFormat::MiddleCenter);
		FVector2 menuTextSize = menuText->GetRelativeSize();
		menuText->SetRelativePosition(
			CoordHelper::RatioCoordToScreen(menuTextSize, FVector2(-0.5f, -0.5f))
		);
		menuBtn->RemoveFromParent();
		menuBtn->AddChildComponent(menuText);
	}
}

void HomeScene::OnExit()
{
	__super::OnExit();
} 
