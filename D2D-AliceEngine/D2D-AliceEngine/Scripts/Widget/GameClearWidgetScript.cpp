#include "GameClearWidgetScript.h"

#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <GameManager/PlayerDataManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Scene/GameScene/GameClearScene.h>
#include <memory>
#include <Component/StatComponent.h>
#include <Scripts/Bike/BikeStatScript.h>

std::wstring GameClearWidgetScript::s_prevScene = L"";

void GameClearWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void GameClearWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void GameClearWidgetScript::Awake()
{
}

void GameClearWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5, 0.5)));

	GetCamera()->AddChildObject(m_owner);

	std::wstring prevScene = SceneManager::GetInstance().GetPrevSceneName();

	if (s_prevScene == L"")
	{
		s_prevScene = prevScene;
	}

	auto background = m_owner->AddComponent<SpriteRenderer>();
	auto cutScene = m_owner->AddComponent<SpriteRenderer>();

	auto skipText = m_owner->AddComponent<TextRenderComponent>();
	auto skipButton = m_owner->AddComponent<ButtonComponent>();

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toMainButton = m_owner->AddComponent<ButtonComponent>();

	m_grade = m_owner->AddComponent<SpriteRenderer>();

	// ========================= //
	auto timeObj = GetWorld()->FindObjectByName<gameObject>(L"PassedTime");
	if (!timeObj) return;
	m_passedTime = timeObj->GetComponent<TextRenderComponent>();
	float timeSec = GamePlayManager::GetInstance().GetPassedTime();
	m_fTime = timeSec;

	int minutes = static_cast<int>(timeSec) / 60;
	int seconds = static_cast<int>(timeSec) % 60;
	int milliseconds = static_cast<int>((timeSec - static_cast<int>(timeSec)) * 100.0f);

	wchar_t buffer[16];
	swprintf(buffer, 16, L"%02d:%02d:%02d", minutes, seconds, milliseconds);
	m_passedTime->SetFontSize(28.0f);
	m_passedTime->SetText(std::wstring(buffer));
	m_passedTime->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_passedTime->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_passedTime->SetTextAlignment(ETextFormat::TopLeft);
	m_passedTime->SetRelativePosition(FVector2(350, 585));
	m_passedTime->SetColor(FColor(0, 234, 255, 255));
	m_passedTime->SetLayer(Define::Disable);

	auto killObj = GetWorld()->FindObjectByName<gameObject>(L"KillCount");
	if (!killObj) return;
	m_killCount = killObj->GetComponent<TextRenderComponent>();

	m_iKill = GamePlayManager::GetInstance().GetKillEnemyAmount();

	m_killCount->SetText(std::to_wstring(m_iKill));
	m_killCount->SetFontSize(28.0f);
	m_killCount->SetTextAlignment(ETextFormat::TopLeft);
	m_killCount->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_killCount->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_killCount->SetRelativePosition(FVector2(350, 553));
	m_killCount->SetColor(FColor(0, 234, 255, 255));
	m_killCount->SetLayer(Define::Disable);

	skipText->SetFontSize(55.0f);
	skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	skipText->SetText(L"건너뛰기");
	skipText->SetColor(FColor::White);
	skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	skipText->SetRelativeScale(FVector2(1, 1));
	skipText->RemoveFromParent();
	skipButton->AddChildComponent(skipText);
	skipText->SetLayer(Define::CutSceneLayer + Define::ButtonTextLayer);
	
	skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	skipButton->SetRelativePosition(FVector2(0, 400));
	skipButton->SetRelativeScale(FVector2(1.f, 1.f));
	skipButton->SetActive(true);
	skipButton->SetLayer(Define::CutSceneLayer + Define::ButtonLayer);

	toMainText->SetFontSize(55.0f);
	toMainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toMainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toMainText->SetText(L"처음 화면으로");
	toMainText->SetColor(FColor::White);
	toMainText->SetRelativePosition(CoordHelper::RatioCoordToScreen(toMainText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	toMainText->SetRelativeScale(FVector2(1, 1));
	toMainText->SetRelativeRotation(0);
	toMainText->SetLayer(Define::Disable);
	toMainText->RemoveFromParent();
	toMainButton->AddChildComponent(toMainText);

	toMainButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toMainButton->SetRelativePosition(FVector2(0, 400));
	toMainButton->SetLayer(Define::Disable);
	toMainButton->SetActive(false);

	// =================== //
	background->LoadData(L"UI/UI_Score.png");
	background->SetDrawType(Define::EDrawType::ScreenSpace);
	background->SetLayer(Define::NormalUILayer);

	cutScene->LoadData(L"CutScene\\Clear\\ending.png");
	cutScene->SetDrawType(EDrawType::ScreenSpace);
	const float targetW = 1920.f;
	const float targetH = 1080.f;
	const float bmpW = cutScene->GetBitmapSizeX();
	const float bmpH = cutScene->GetBitmapSizeY();

	if (bmpW > 0.f && bmpH > 0.f)
	{
		const FVector2 scale(targetW / bmpW, targetH / bmpH);
		cutScene->SetRelativeScale(scale);
		cutScene->SetLayer(CutSceneLayer);
	}

	m_grade->LoadData(L"UI\\UI_Grade.png");
	m_grade->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f - 60 ,-850));

	skipButton->SetStateAction(Define::EButtonState::Hover, [skipButton]()
		{
			skipButton->StartHoverPulse(0.8f, 0.04f);
			skipButton->StartEffectAnimation(0.3f, 1.2f, FColor::White);
		});

	skipButton->SetStateAction(Define::EButtonState::HoverLeave, [skipButton]()
		{
			skipButton->StopHoverPulse();
			skipButton->StartEffectAnimation(0.2f, 0.0f, FColor::White);
		});

	skipButton->SetStateAction(Define::EButtonState::Release, [skipButton]()
		{
			skipButton->StopHoverPulse();
			skipButton->StartEffectAnimation(0.1f, 0.0f, FColor::White);
		});

	skipButton->SetStateAction(EButtonState::Pressed, [
		weak = WeakFromThis<GameClearWidgetScript>(),
		skipButton, skipText, cutScene,
		toMainButton, toMainText
	] {
		if (weak.expired())return;
			
		cutScene->SetLayer(Define::Disable);
		skipButton->SetLayer(Define::Disable);
		skipText->SetLayer(Define::Disable);

		weak.Get()->m_passedTime->SetLayer(Define::NormalTextLayer);
		weak.Get()->m_killCount->SetLayer(Define::NormalTextLayer);

		skipButton->SetActive(false);

		toMainButton->SetActive(true);
		toMainButton->SetLayer(Define::ButtonLayer);
		toMainText->SetLayer(Define::ButtonTextLayer);
		});

	if (s_prevScene != Define::Scene_Stage3)
	{
		cutScene->SetLayer(Define::Disable);
		skipButton->SetLayer(Define::Disable);
		skipText->SetLayer(Define::Disable);
		skipButton->SetActive(false);

		toMainButton->SetActive(true);
		toMainButton->SetLayer(Define::ButtonLayer);
		toMainText->SetLayer(Define::ButtonTextLayer);
	}

	// ====================== Delegate
	toMainButton->SetStateAction(Define::EButtonState::Hover, [toMainButton]()
		{
			toMainButton->StartHoverPulse(0.8f, 0.04f);
			toMainButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::HoverLeave, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::Release, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::Pressed, [] {
		
		SceneManager::ChangeScene(L"TitleScene");
		});

	//if()
	
	TimerManager::GetInstance().ClearTimer(m_killHandle);
	TimerManager::GetInstance().ClearTimer(m_timeHandle);
	TimerManager::GetInstance().ClearTimer(m_gradeHandle);

	TimerManager::GetInstance().SetTimer(
		m_killHandle,
		[weak = WeakFromThis<GameClearWidgetScript>()]() mutable {
			if (weak.expired())return;
			weak.Get()->m_killCount->SetLayer(Define::PopupObjectLayer);
		},
		0.1f,
		false,
		1.0f
	);

	TimerManager::GetInstance().SetTimer(
		m_timeHandle,
		[weak = WeakFromThis<GameClearWidgetScript>()]() mutable {
			if (weak.expired())return;
			weak.Get()->m_passedTime->SetLayer(Define::PopupObjectLayer);
		},
		0.1f,
		false,
		2.0f
	);

	TimerManager::GetInstance().SetTimer(
		m_gradeHandle,
		[weak = WeakFromThis<GameClearWidgetScript>()]() mutable {
			if (weak.expired())return;
			weak.Get()->m_grade->SetLayer(Define::PopupObjectLayer);
		},
		0.1f,
		false,
		3.0f
	);
}

void GameClearWidgetScript::OnEnd()
{
}

void GameClearWidgetScript::OnDestroy()
{
	TimerManager::GetInstance().ClearTimer(m_killHandle);
	TimerManager::GetInstance().ClearTimer(m_timeHandle);
	TimerManager::GetInstance().ClearTimer(m_gradeHandle);
}
