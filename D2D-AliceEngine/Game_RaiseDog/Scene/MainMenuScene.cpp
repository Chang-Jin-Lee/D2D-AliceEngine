#include "MainMenuScene.h"
#include <Component/TextRenderComponent.h>
#include <Manager/SceneManager.h>
#include <Core/Input.h>
#include <Object/gameObject.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Helper/ParticleHelper.h>
#include <Manager/D2DRenderManager.h>
#include <Component/SpriteRenderer.h>
#include <Component/VideoComponent.h>
#include <GameApp.h>

void MainMenuScene::Initialize()
{
	__super::Initialize();
}

void MainMenuScene::Release()
{
	__super::Release();
}

void MainMenuScene::Update()
{
	__super::Update();

	// 빈 공간 클릭 이펙트 (UI 위가 아닐 때)
	if (Input::IsMouseLeftPressed() && !m_mouseOverUI)
	{
		ParticleHelper::SpawnParticleClickL(Input::GetMousePosition(), Define::Effect_Texture_Collision);
	}
}

void MainMenuScene::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget_mainmenu");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <메인 메뉴> \n"
		L" 귀여운 강아지와 함께하는 왕쨩 더비!"
	);
	m_widget->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(40, 40));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(28.0f);

	FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
	FVector2 center(0,0);

	// Sarang 메인 비디오 표시 (월드 스페이스, 0,0이 화면 중앙)
	{
		auto sarangObj = NewObject<gameObject>(L"Sarang");
		auto* video = sarangObj->AddComponent<VideoComponent>();
		video->LoadData(L"WangZZang/mainVideo.mp4", 30, L"jpg", 95, false);
		video->SetLayer(Define::NormalUILayer);
		video->GetRelativeTransform()->SetPosition(center + FVector2(0,50));
		FVector2 bmp(video->GetBitmapSizeX(), video->GetBitmapSizeY());
		float targetH = app.y * 0.6f;
		float scale = ((bmp.y > 1.f) ? targetH / bmp.y : 1.f ) * 0.35f;
		video->SetRelativeScale(FVector2(scale, scale));
		video->Play();
	}

	// 우마무스메풍 버튼: START, EXIT (D2D 좌표: 좌상단 0,0)
	{
		FVector2 startCenter(app.x * 0.5f, app.y * 0.5f + 190);
		auto startBtnObj = NewObject<gameObject>(L"btn_start");
		m_startBtn = startBtnObj->AddComponent<ButtonComponent>();
		m_startBtn->LoadData(Define::EButtonState::Idle, L"UI/Btn_Start_Idle.png");
		m_startBtn->LoadData(Define::EButtonState::Hover, L"UI/Btn_Start_Hover.png");
		m_startBtn->LoadData(Define::EButtonState::Pressed, L"UI/Btn_Start_Pressed.png");
		m_startBtn->SetFallbackSize(FVector2(260, 70));
		m_startBtn->SetFallbackColor(FColor(80, 160, 255, 255));
		m_startBtn->SetLayer(Define::ButtonLayer);
		FVector2 startBtnSize = m_startBtn->GetRelativeSize();
		m_startBtn->SetRelativePosition(CoordHelper::RatioCoordToScreen(startBtnSize, FVector2(0.0f, 0.0f)) + startCenter);
		m_startBtn->SetStateAction(Define::EButtonState::Release, [this]() {
			ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
			SceneManager::ChangeScene(L"Home");
		});

		auto* startText = startBtnObj->AddComponent<TextRenderComponent>();
		startText->SetText(L"게임 시작");
		startText->SetFontSize(24.0f);
		startText->SetColor(FColor(255, 255, 255, 255));
		startText->SetTextAlignment(ETextFormat::MiddleCenter);
		startText->SetLayer(Define::ButtonTextLayer);
		FVector2 startTextSize = startText->GetRelativeSize();
		startText->SetRelativePosition(
			CoordHelper::RatioCoordToScreen(startTextSize, FVector2(-0.5f, -0.5f))
		);
		startText->RemoveFromParent();
		m_startBtn->AddChildComponent(startText);

		FVector2 exitCenter(app.x * 0.5f, app.y * 0.5f + 270);
		auto exitBtnObj = NewObject<gameObject>(L"btn_exit");
		m_exitBtn = exitBtnObj->AddComponent<ButtonComponent>();
		m_exitBtn->LoadData(Define::EButtonState::Idle, L"UI/Btn_Exit_Idle.png");
		m_exitBtn->LoadData(Define::EButtonState::Hover, L"UI/Btn_Exit_Hover.png");
		m_exitBtn->LoadData(Define::EButtonState::Pressed, L"UI/Btn_Exit_Pressed.png");
		m_exitBtn->SetFallbackSize(FVector2(260, 70));
		m_exitBtn->SetFallbackColor(FColor(255, 120, 150, 255));
		m_exitBtn->SetLayer(Define::ButtonLayer);
		FVector2 exitBtnSize = m_exitBtn->GetRelativeSize();
		m_exitBtn->SetRelativePosition(CoordHelper::RatioCoordToScreen(exitBtnSize, FVector2(0.f, 0.f)) + exitCenter);
		m_exitBtn->SetStateAction(Define::EButtonState::Release, []() {
			ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
			GameApp::GetInstance().GameQuit();
			//PostQuitMessage(0);
		});

		auto* exitText = exitBtnObj->AddComponent<TextRenderComponent>();
		exitText->SetText(L"게임 종료");
		exitText->SetFontSize(24.0f);
		exitText->SetColor(FColor(255,255,255,255));
		exitText->SetTextAlignment(ETextFormat::MiddleCenter);
		exitText->SetLayer(Define::ButtonTextLayer);
		FVector2 exitTextSize = exitText->GetRelativeSize();
		exitText->SetRelativePosition(
			CoordHelper::RatioCoordToScreen(exitTextSize, FVector2(-0.5f, -0.5f))
		);
		exitText->RemoveFromParent();
		m_exitBtn->AddChildComponent(exitText);
	}
}

void MainMenuScene::OnExit()
{
	__super::OnExit();
} 
