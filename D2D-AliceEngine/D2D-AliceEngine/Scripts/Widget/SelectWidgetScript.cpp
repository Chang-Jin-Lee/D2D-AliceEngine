#include "SelectWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>
#include <Component/ProgressBarComponent.h>
#include <Component/AudioComponent.h>
#include <Component/ButtonComponent.h>
#include <Component/InputComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>

void SelectWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void SelectWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	m_progress->SetProgress(m_pressValue / 500.0f);

	FVector2 mousePos = Input::GetMousePosition();

	m_progress->SetRelativePosition(FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 450));
	m_progressSprite->SetRelativePosition(FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 450));
	m_guideText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_guideText->GetRelativeSize(), FVector2(-0.5, -0.5)) +
		FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 400));

	m_seconds = deltaSeconds;
}

void SelectWidgetScript::Awake()
{
}

void SelectWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	SetPressValue(0);
	m_isInStage1 = false;

	auto background = m_owner->AddComponent<SpriteRenderer>();
	auto tutorial = m_owner->AddComponent<VideoComponent>();

	auto input = m_owner->AddComponent<InputComponent>();
	m_progressSprite = m_owner->AddComponent<SpriteRenderer>();
	m_progress = m_owner->AddComponent<ProgressBarComponent>();

	m_guideText = m_owner->AddComponent<TextRenderComponent>();

	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto closeText = m_owner->AddComponent<TextRenderComponent>();

	//auto uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	//uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::UI);

	// Close
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	// Skip
	auto skipText = m_owner->AddComponent<TextRenderComponent>();
	auto skipButton = m_owner->AddComponent<ButtonComponent>();

	// Select Stage Button
	auto stage1 = m_owner->AddComponent<ButtonComponent>();
	auto stage2 = m_owner->AddComponent<ButtonComponent>();
	auto stage3 = m_owner->AddComponent<ButtonComponent>();

	auto stage1Sprite = m_owner->AddComponent<SpriteRenderer>();
	auto stage2Sprite = m_owner->AddComponent<SpriteRenderer>();
	auto stage3Sprite = m_owner->AddComponent<SpriteRenderer>();


	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	m_sound = bgmObj->GetComponent<AudioComponent>();

	// ===================================================== //
	background->LoadData(L"BackGround\\Background_Select.png");
	background->SetDrawType(Define::EDrawType::ScreenSpace);
	FVector2 backgroundSize = background->GetRelativeSize();
	background->SetRelativePosition(CoordHelper::RatioCoordToScreen(backgroundSize, FVector2(0, 0)));
	background->SetRelativeScale(FVector2(1, 1));
	
	tutorial->LoadData(L"BackGround\\movie_tutorial.mp4", 14, L"jpg", 95, true);
	tutorial->SetLayer(Define::Disable);
	tutorial->SetRelativePosition(FVector2(-960, -550));

	//continueText->SetFontSize(70.0f);
	//continueText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//continueText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//continueText->SetText(L"임무 선택");
	//continueText->SetColor(FColor(0, 234, 255, 255));
	//continueText->SetRelativePosition(
	//	CoordHelper::RatioCoordToScreen(continueText->GetRelativeSize(),FVector2(-0.5,-0.5))
	//	+FVector2(0,-350));
	//continueText->m_layer = Define::NormalTextLayer;

	// ======================== closeText
	closeText->SetFontSize(55.0f);
	closeText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	closeText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	closeText->SetText(L"돌아가기");
	closeText->SetColor(FColor::White);
	FVector2 closeTextRectSize = closeText->GetRelativeSize();
	closeText->SetRelativePosition(CoordHelper::RatioCoordToScreen(closeTextRectSize, FVector2(-0.5, -0.5)));
	closeText->SetRelativeScale(FVector2(1, 1));
	closeText->SetRelativeRotation(0);
	closeText->SetLayer(Define::NormalTextLayer);
	closeText->RemoveFromParent();
	closeButton->AddChildComponent(closeText);

	// ===================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	closeButton->SetRelativePosition(FVector2(0, 400));

	// ======================== skipText
	//skipText->SetFontSize(55.0f);
	//skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//skipText->SetText(L"건너뛰기");
	//skipText->SetColor(FColor::White);
	//FVector2 skipTextRectSize = skipText->GetRelativeSize();
	//skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextRectSize, FVector2(-0.5, -0.5)));
	//skipText->SetRelativeScale(FVector2(1, 1));
	//skipText->SetRelativeRotation(0);
	//skipText->m_layer = Define::Disable;
	//skipButton->SetActive(false);
	//skipText->RemoveFromParent();
	//skipButton->AddChildComponent(skipText);

	// ======================== skipButton
	//skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	//skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	//skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	//skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	//skipButton->SetRelativePosition(FVector2(0, 350));
	//skipButton->SetRelativeScale(FVector2(1, 1));
	//skipButton->m_layer = Define::Disable;

	// ======================= GuideText
	m_guideText->SetFontSize(25.0f);
	m_guideText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_guideText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_guideText->SetText(L"[SPACE]를 길게 눌러서 튜토리얼 건너뛰기");
	m_guideText->SetColor(FColor::White);
	m_guideText->SetLayer(Define::Disable);

	// ===================== stage1
	stage1->LoadData(Define::EButtonState::Idle, L"UI\\StageButton_Idle.png");
	stage1->LoadData(Define::EButtonState::Hover, L"UI\\StageButton_Idle.png");
	stage1->LoadData(Define::EButtonState::Pressed, L"UI\\StageButton_Pressed.png");
	stage1->LoadData(Define::EButtonState::Release, L"UI\\StageButton_Idle.png");
	FVector2 stage1Size = stage1->GetRelativeSize();
	stage1->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage1Size, FVector2(0 ,0))
		+ FVector2(-570, 0)
	);
	stage1->SetRelativeScale(FVector2(1, 1));
	stage1->SetLayer(Define::NormalTextLayer);

	stage1Sprite->SetRelativeScale(FVector2(0.35f));
	stage1Sprite->SetDrawType(Define::EDrawType::ScreenSpace);
	stage1Sprite->LoadData(L"CutScene/Stage1/stage_1_scene01.png");
	stage1Sprite->RemoveFromParent();
	stage1->AddChildComponent(stage1Sprite);
	FVector2 stage1SpriteSize = stage1Sprite->GetRelativeSize();
	stage1Sprite->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage1SpriteSize, FVector2(0, 0))
		+ FVector2(0, 0)
	);
	stage1Sprite->SetLayer(Define::NormalTextLayer+1);

	stage2Sprite->SetRelativeScale(FVector2(0.35f));
	stage2Sprite->SetDrawType(Define::EDrawType::ScreenSpace);
	stage2Sprite->LoadData(L"CutScene/Stage2/stage_2_scene03.png");
	stage2Sprite->RemoveFromParent();
	stage2->AddChildComponent(stage2Sprite);
	FVector2 stage2SpriteSize = stage2Sprite->GetRelativeSize();
	stage2Sprite->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage2SpriteSize, FVector2(0, 0))
		+ FVector2(0, 0)
	);
	stage2Sprite->SetLayer(Define::NormalTextLayer + 1);

	stage3Sprite->SetRelativeScale(FVector2(0.35f));
	stage3Sprite->SetDrawType(Define::EDrawType::ScreenSpace);
	stage3Sprite->LoadData(L"CutScene/Stage3/stage_3_scene05.png");
	stage3Sprite->RemoveFromParent();
	stage3->AddChildComponent(stage3Sprite);
	FVector2 stage3SpriteSize = stage3Sprite->GetRelativeSize();
	stage3Sprite->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage3SpriteSize, FVector2(0, 0))
		+ FVector2(0, 0)
	);
	stage3Sprite->SetLayer(Define::NormalTextLayer + 1);

	// ===================== stage2
	stage2->LoadData(Define::EButtonState::Idle, L"UI\\StageButton_Idle.png");
	stage2->LoadData(Define::EButtonState::Hover, L"UI\\StageButton_Idle.png");
	stage2->LoadData(Define::EButtonState::Pressed, L"UI\\StageButton_Pressed.png");
	stage2->LoadData(Define::EButtonState::Release, L"UI\\StageButton_Idle.png");
	stage2->SetRelativePosition(FVector2(0,0));
	stage2->SetLayer(Define::NormalTextLayer);

	// ===================== stage3
	stage3->LoadData(Define::EButtonState::Idle, L"UI\\StageButton_Idle.png");
	stage3->LoadData(Define::EButtonState::Hover, L"UI\\StageButton_Idle.png");
	stage3->LoadData(Define::EButtonState::Pressed, L"UI\\StageButton_Pressed.png");
	stage3->LoadData(Define::EButtonState::Release, L"UI\\StageButton_Idle.png");
	FVector2 stage3Size = stage3->GetRelativeSize();
	stage3->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage3Size, FVector2(0, 0))
		+ FVector2(570, 0)
	);
	stage3->SetRelativeScale(FVector2(1, 1));
	stage3->SetLayer(Define::NormalTextLayer);

	// ========= Progress
	m_progressSprite->LoadData(L"UI\\PressValue.png");
	m_progressSprite->SetLayer(Define::Disable);
	m_progressSprite->SetDrawType(EDrawType::ScreenSpace);
	m_progressSprite->SetRelativePosition(FVector2(0, 0));
	
	m_progress->LoadData(L"UI\\ProgressBar.png");
	m_progress->SetDrawType(EDrawType::ScreenSpace);
	m_progress->SetRelativePosition(FVector2(0, 0));
	m_progress->SetLayer(Define::Disable);
	m_progress->SetProgress(m_pressValue);

	// ====================== Delegate & Hover Effects
	
	// stage1 Button 효과 (초록색 글로우 - 쉬운 스테이지)
	stage1->SetStateAction(Define::EButtonState::Hover, [stage1]()
	{
		stage1->StartHoverPulse(3.f, 0.01f);
		stage1->StartEffectAnimation(0.3f, 1.2f, FColor::White);
	});

	stage1->SetStateAction(Define::EButtonState::HoverLeave, [stage1]()
	{
		stage1->StopHoverPulse();
		stage1->StartEffectAnimation(0.2f, 0.0f, FColor::White);
	});

	stage1->SetStateAction(Define::EButtonState::Release, [stage1]()
	{
		stage1->StopHoverPulse();
		stage1->StartEffectAnimation(0.1f, 0.0f, FColor::White);
	});

	stage1->SetStateAction(Define::EButtonState::Pressed, [this, closeButton, tutorial, skipButton, skipText,
		stage1, stage2, stage3
	]{
		
		//skipButton->m_layer = Define::PopupButtonLayer;
		//skipButton->SetActive(true);
		//skipText->m_layer = Define::PopupTextLayer;

		tutorial->SetLayer(Define::PopupLayer);
		tutorial->Play();

		m_sound->PlayByName(L"Tutorial");
		closeButton->SetActive(false);

		stage1->SetActive(false);
		stage2->SetActive(false);
		stage3->SetActive(false);

		m_progressSprite->SetLayer(Define::PopupPopLayer);
		m_progress->SetLayer(Define::PopupObjectLayer);
		m_guideText->SetLayer(Define::PopupTextLayer);

		m_isInStage1 = true;

		//SceneManager::ChangeScene(Define::Scene_Stage1);
		});

	//skipButton->SetStateAction(Define::EButtonState::Hover, [skipButton]()
	//	{
	//		skipButton->StartHoverPulse(0.8f, 0.04f);
	//		skipButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	//	});

	//skipButton->SetStateAction(Define::EButtonState::HoverLeave, [skipButton]()
	//	{
	//		skipButton->StopHoverPulse();
	//		skipButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	//	});

	//skipButton->SetStateAction(Define::EButtonState::Release, [skipButton]()
	//	{
	//		skipButton->StopHoverPulse();
	//		skipButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	//	});

	//skipButton->SetStateAction(Define::EButtonState::Pressed, []()
	//	{
	//		OutputDebugStringW(L"SetAction click!\n");
	//		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
	//		m_sound->StopByName(L"Tutorial");
	//		
	//		SceneManager::ChangeScene(Define::Scene_Stage1);
	//	});

	// stage2 Button 효과 (노란색 글로우 - 보통 스테이지)
	stage2->SetStateAction(Define::EButtonState::Hover, [stage2]()
	{
		stage2->StartHoverPulse(3.f, 0.01f);
		stage2->StartEffectAnimation(0.3f, 1.2f, FColor::White);
	});

	stage2->SetStateAction(Define::EButtonState::HoverLeave, [stage2]()
	{
		stage2->StopHoverPulse();
		stage2->StartEffectAnimation(0.2f, 0.0f, FColor::White);
	});

	stage2->SetStateAction(Define::EButtonState::Release, [stage2]()
	{
		stage2->StopHoverPulse();
		stage2->StartEffectAnimation(0.1f, 0.0f, FColor::White);
	});

	stage2->SetStateAction(Define::EButtonState::Pressed, [] {

		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(Define::Scene_Stage2);
		});

	// stage3 Button 효과 (빨간색 글로우 - 어려운 스테이지/보스)
	stage3->SetStateAction(Define::EButtonState::Hover, [stage3]()
	{
		stage3->StartHoverPulse(3.f, 0.01f);
		stage3->StartEffectAnimation(0.3f, 1.2f, FColor::White);
	});

	stage3->SetStateAction(Define::EButtonState::HoverLeave, [stage3]()
	{
		stage3->StopHoverPulse();
		stage3->StartEffectAnimation(0.2f, 0.0f, FColor::White);
	});

	stage3->SetStateAction(Define::EButtonState::Release, [stage3]()
	{
		stage3->StopHoverPulse();
		stage3->StartEffectAnimation(0.1f, 0.0f, FColor::White);
	});

	stage3->SetStateAction(Define::EButtonState::Pressed, [] {
		
		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(Define::Scene_Stage3);
		});

	// closeButton 효과 (주황색 글로우)
	closeButton->SetStateAction(Define::EButtonState::Hover, [closeButton]()
	{
		closeButton->StartHoverPulse(0.8f, 0.04f);
		closeButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	});

	closeButton->SetStateAction(Define::EButtonState::HoverLeave, [closeButton]()
	{
		closeButton->StopHoverPulse();
		closeButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	});

	closeButton->SetStateAction(Define::EButtonState::Release, [closeButton]()
	{
		closeButton->StopHoverPulse();
		closeButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	});

	closeButton->SetStateAction(Define::EButtonState::Pressed,[] {

		SceneManager::ChangeScene(L"TitleScene");
		});

	input->SetAction(input->GetHandle(), [this] {
		SkipInput(m_seconds);
		});
}

void SelectWidgetScript::OnEnd()
{
}

void SelectWidgetScript::OnDestroy()
{
}

void SelectWidgetScript::SkipInput(float deltaSeconds)
{
	if (m_isInStage1 && Input::IsKeyDown(VK_SPACE))
	{
		m_pressValue += 4.5f * deltaSeconds * 300;

		if (m_pressValue >= 500.0f)
		{
			m_sound->StopByName(L"Tutorial");
   			SceneManager::ChangeScene(Define::Scene_Stage1);
		}
	}

	m_pressValue -= 1.5f * deltaSeconds * 300;
	if (m_pressValue <= 0.0f) m_pressValue = 0.0f;
}
