#include "ParkScene.h"
#include <Component/TextRenderComponent.h>
#include <Manager/SceneManager.h>
#include <Core/Input.h>
#include <Object/gameObject.h>
#include <Component/ButtonComponent.h>
#include <Helper/ParticleHelper.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/CoordHelper.h>

void ParkScene::Initialize()
{
	__super::Initialize();
}

void ParkScene::Release()
{
	__super::Release();
}

void ParkScene::Update()
{
	__super::Update();

	if (Input::IsMouseLeftPressed())
	{
		ParticleHelper::SpawnParticleClickL(Input::GetMousePosition(), Define::Effect_Texture_Collision);
	}
}

void ParkScene::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget_park");
	auto* title = m_widget->AddComponent<TextRenderComponent>();
	title->SetText(
		L" <공원> \n"
		L" 왕쨩 더비 훈련장"
	);
	title->SetRelativePosition(FVector2(180, 40));
	title->SetFontSize(26.0f);

	FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
	FVector2 startCenter(app.x * 0.5f, app.y * 0.5f + 20);

	// 버튼: 집으로 가기 (센터 정렬)
	{
		auto homeBtnObj = NewObject<gameObject>(L"btn_to_home");
		auto* homeBtn = homeBtnObj->AddComponent<ButtonComponent>();
		homeBtn->LoadData(Define::EButtonState::Idle, L"UI/Btn_Home_Idle.png");
		homeBtn->LoadData(Define::EButtonState::Hover, L"UI/Btn_Home_Hover.png");
		homeBtn->LoadData(Define::EButtonState::Pressed, L"UI/Btn_Home_Pressed.png");
		homeBtn->SetFallbackSize(FVector2(260, 70));
		homeBtn->SetFallbackColor(FColor(255, 160, 120, 255));
		FVector2 btnSize = homeBtn->GetRelativeSize();
		homeBtn->SetRelativePosition(CoordHelper::RatioCoordToScreen(btnSize, FVector2(0.f, 0.f)) + startCenter);
		homeBtn->SetStateAction(Define::EButtonState::Release, []() {
			ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
			SceneManager::ChangeScene(L"Home");
		});

		auto* label = homeBtnObj->AddComponent<TextRenderComponent>();
		label->SetText(L"집으로");
		label->SetFontSize(24.0f);
		label->SetColor(FColor(255,255,255,255));
		label->SetTextAlignment(ETextFormat::MiddleCenter);
		FVector2 textSize = label->GetRelativeSize();
		label->SetRelativePosition(CoordHelper::RatioCoordToScreen(textSize, FVector2(-0.5f, -0.5f)));
		label->RemoveFromParent();
		homeBtn->AddChildComponent(label);
	}
}

void ParkScene::OnExit()
{
	__super::OnExit();
} 
