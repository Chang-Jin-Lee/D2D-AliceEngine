#include "GameOverWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <System/ScriptSystem.h>

#include <Helpers/CoordHelper.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>

#include <Scene/Scene.h>
 #include <Component/ButtonComponent.h>

#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>

void GameOverWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void GameOverWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	m_sec += TimerManager::GetInstance().unscaledDeltaTime * 10;

	if (m_sec >= 1.0f) m_isPop = true;

	if (m_isPop)
	{
		m_scale += TimerManager::GetInstance().unscaledDeltaTime * 10;

		if (m_scale >= 1.0f) m_scale = 1.0f;

		m_errorPopup->SetLayer(Define::PopupLayer);
		m_toMainButton->SetLayer(Define::PopupButtonLayer);
		m_toRestartButton->SetLayer(Define::PopupButtonLayer);
		m_errorPopup->SetRelativeScale(m_scale);
	}
}

void GameOverWidgetScript::Awake()
{
}

void GameOverWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	auto background = m_owner->AddComponent<SpriteRenderer>();

	m_errorPopup = m_owner->AddComponent<SpriteRenderer>();

	m_toRestartButton = m_owner->AddComponent<ButtonComponent>();
	m_toMainButton = m_owner->AddComponent<ButtonComponent>();

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toRestartText = m_owner->AddComponent<TextRenderComponent>();

	// ==================== //
	background->LoadData(L"UI\\UI_Score_FAIL.png");
	background->SetDrawType(EDrawType::ScreenSpace);
	background->SetLayer(Define::NormalUILayer);

	m_errorPopup->LoadData(L"UI\\UI_ERROR_Popup.png");
	m_errorPopup->SetDrawType(EDrawType::ScreenSpace);
	m_errorPopup->SetLayer(Define::Disable);

	toRestartText->SetFontSize(45.f);
	toRestartText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toRestartText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toRestartText->SetText(L"예");
	toRestartText->SetColor(FColor::Black);
	toRestartText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toRestartText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toRestartText->SetLayer(Define::PopupTextLayer);
	toRestartText->RemoveFromParent();
	m_toRestartButton->AddChildComponent(toRestartText);

	toMainText->SetFontSize(45.f);
	toMainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toMainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toMainText->SetText(L"아니오");
	toMainText->SetColor(FColor::Black);
	toMainText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toMainText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toMainText->SetLayer(Define::PopupTextLayer);
	toMainText->RemoveFromParent();
	m_toMainButton->AddChildComponent(toMainText);

	// ==================== Button
	m_toRestartButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_ERRORButton.png");
	m_toRestartButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_ERRORButton.png");
	m_toRestartButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ERRORButton.png");
	m_toRestartButton->LoadData(Define::EButtonState::Release, L"UI\\UI_ERRORButton.png");
	m_toRestartButton->SetLayer(Define::Disable);
	m_toRestartButton->SetDrawType(EDrawType::ScreenSpace);
	m_toRestartButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_toRestartButton->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(-180 + 100, 100));

	m_toMainButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_ERRORButton.png");
	m_toMainButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_ERRORButton.png");
	m_toMainButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ERRORButton.png");
	m_toMainButton->LoadData(Define::EButtonState::Release, L"UI\\UI_ERRORButton.png");
	m_toMainButton->SetDrawType(EDrawType::ScreenSpace);
	m_toMainButton->SetLayer(Define::Disable);
	m_toMainButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_toMainButton->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(180, 100));

	// ==================== Delegate
	m_toRestartButton->SetStateAction(Define::EButtonState::Hover, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toRestartButton->StartHoverPulse(0.8f, 0.04f);
			weak.Get()->m_toRestartButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	m_toRestartButton->SetStateAction(Define::EButtonState::HoverLeave, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toRestartButton->StopHoverPulse();
			weak.Get()->m_toRestartButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	m_toRestartButton->SetStateAction(Define::EButtonState::Release, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toRestartButton->StopHoverPulse();
			weak.Get()->m_toRestartButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	m_toRestartButton->SetStateAction(Define::EButtonState::Pressed, [] {
		std::wstring prevScene = SceneManager::GetPrevSceneName();
		SceneManager::ChangeScene(prevScene);
		});

	m_toMainButton->SetStateAction(Define::EButtonState::Hover, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toMainButton->StartHoverPulse(0.8f, 0.04f);
			weak.Get()->m_toMainButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	m_toMainButton->SetStateAction(Define::EButtonState::HoverLeave, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toMainButton->StopHoverPulse();
			weak.Get()->m_toMainButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	m_toMainButton->SetStateAction(Define::EButtonState::Release, [weak = WeakFromThis<GameOverWidgetScript>()]()
		{
			if (weak.expired())return;
			weak.Get()->m_toMainButton->StopHoverPulse();
			weak.Get()->m_toMainButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	m_toMainButton->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"TitleScene");
		});

	/*TimerManager::GetInstance().ClearTimer(m_fadeHandle);
	TimerManager::GetInstance().SetTimer(
		m_fadeHandle,
		[errorPopup]() mutable {
			float scale = 0.0f;

			scale += 0.1f * TimerManager::GetInstance().unscaledDeltaTime;

			if (scale > 1.0f) scale = 1.0f;

			errorPopup->SetRelativeScale(scale);


		},
		0.1f,
		true,
		1.0f
		);*/

	//TimerManager::GetInstance().ClearTimer(m_fadeHandle);
	//TimerManager::GetInstance().SetTimer(
	//	m_fadeHandle,
	//	[weak = WeakFromThis<GameOverWidgetScript>(), errorPopup]() mutable {
	//		if (weak.expired()) return;
	//		//float scale = 0.0f;

	//		weak.Get()->scale += 0.1f * TimerManager::GetInstance().unscaledDeltaTime;

	//		if (weak.Get()->scale > 1.0f) weak.Get()->scale = 1.0f;

	//		errorPopup->SetRelativeScale(weak.Get()->scale);


	//	},
	//	0.1f,	// 간격(false면 무의미해짐)
	//	false,
	//	1.0f
	//);
}

void GameOverWidgetScript::OnEnd()
{
}

void GameOverWidgetScript::OnDestroy()
{
	//TimerManager::GetInstance().ClearTimer(m_fadeHandle);
}
