#include "TitleWidgetScript.h"
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
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Component/ProgressBarComponent.h>

#include <Application.h>
//#include <Scripts/Spine2D/SpineScript.h>

void TitleWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void TitleWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TitleWidgetScript::Awake()
{
}

void TitleWidgetScript::OnStart()
{
	//if (Scene* scene = GetWorld())
	//{
	//	spineObject = scene->NewObject<gameObject>(L"spineObject");
	//	spine = spineObject->AddComponent<SpineScript>();
	//	spine->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
	//	if (spine->spineRenderer)
	//	{
	//		spine->spineRenderer->SetAnimation("Idle");
	//		spine->spineRenderer->SetPosition(FVector2(-200, 0));
	//		spine->spineRenderer->SetRendered(false);
	//	}
	//
	//	spineObject2 = scene->NewObject<gameObject>(L"spineObject");
	//	spine2 = spineObject2->AddComponent<SpineScript>();
	//	spine2->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
	//	if (spine2->spineRenderer)
	//	{
	//		spine2->spineRenderer->SetAnimation("Idle");
	//		spine2->spineRenderer->SetPosition(FVector2(100, 0));
	//		spine2->spineRenderer->SetRendered(false);
	//	}
	//}

	m_owner = GetOwner();
	m_owner->transform()->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f,0.5f)));

	float guargeSize = 1.0f;
	float soundUISize = 1.0f;

	auto background = m_owner->AddComponent<SpriteRenderer>();

	auto textLogo = m_owner->AddComponent<SpriteRenderer>();

	auto startText = m_owner->AddComponent<TextRenderComponent>();
	auto startButton = m_owner->AddComponent<ButtonComponent>();
	//startButton->RemoveFromParent();
	//GetCamera()->AddChildObject(startButton);

	// 아트 리소스 확인하기
	//auto artBookText = m_owner->AddComponent<TextRenderComponent>();
	//auto artBookButton = m_owner->AddComponent<ButtonComponent>();

	auto artBookText = m_owner->AddComponent<TextRenderComponent>();
	auto artBookButton = m_owner->AddComponent<ButtonComponent>();

	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto continueButton = m_owner->AddComponent<ButtonComponent>();
	auto continueTabText = m_owner->AddComponent<TextRenderComponent>();

	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto optionButton = m_owner->AddComponent<ButtonComponent>();
	auto optionTabText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabBGMText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabSFXText = m_owner->AddComponent<TextRenderComponent>();

	auto staffText = m_owner->AddComponent<TextRenderComponent>();
	auto staffButton = m_owner->AddComponent<ButtonComponent>();
	auto staffTabText = m_owner->AddComponent<TextRenderComponent>();
	auto staffNameText = m_owner->AddComponent<TextRenderComponent>();

	auto quitText = m_owner->AddComponent<TextRenderComponent>();
	auto quitButton = m_owner->AddComponent<ButtonComponent>();

	//auto spineShowText = m_owner->AddComponent<TextRenderComponent>();
	//auto spineShowButton = m_owner->AddComponent<ButtonComponent>();

	auto closeText = m_owner->AddComponent<TextRenderComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();
	auto smallClose = m_owner->AddComponent<ButtonComponent>();

	auto mainTitle = m_owner->AddComponent<TextRenderComponent>();
	auto subTitle = m_owner->AddComponent<TextRenderComponent>();

	auto bgmControl = m_owner->AddComponent<ProgressBarComponent>();
	auto sfxControl = m_owner->AddComponent<ProgressBarComponent>();

	auto bgmPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto bgmMinusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxMinusButton = m_owner->AddComponent<ButtonComponent>();

	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	auto sound = bgmObj->GetComponent<AudioComponent>();
	
	float bgmVolume = AudioManager::GetInstance().GetBGMVolume();
	float sfxVolume = AudioManager::GetInstance().GetSFXVolume();

    auto soundControl = m_owner->AddComponent<SpriteRenderer>();
    soundControl->LoadData(L"UI\\UI_SoundController.png");
    soundControl->SetLayer(Define::Disable);
    soundControl->SetRelativeScale(soundUISize);
    soundControl->SetDrawType(EDrawType::ScreenSpace);
    // 중앙 배치
    soundControl->SetRelativePosition(FVector2(0.f, 0.f));

	bgmControl->SetDrawType(Define::EDrawType::ScreenSpace);
	bgmControl->LoadData(L"UI\\ControlBar.png");
	bgmControl->SetType(EProgressBarType::Linear);
	bgmControl->SetRelativeScale(FVector2(1, 1));
	bgmControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(bgmControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(5, 3));
	bgmControl->SetProgress(bgmVolume);
	bgmControl->SetLayer(Define::Disable);

	sfxControl->SetDrawType(Define::EDrawType::ScreenSpace);
	sfxControl->LoadData(L"UI\\ControlBar.png");
	sfxControl->SetType(EProgressBarType::Linear);
	sfxControl->SetRelativeScale(FVector2(1, 1));
	sfxControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(sfxControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(5, 66));
	sfxControl->SetProgress(sfxVolume);
	sfxControl->SetLayer(Define::Disable);

	//auto uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	//uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);	// UISound is Included in SFX.

	//auto tutorial = m_owner->AddComponent<VideoComponent>();
	//tutorial->LoadData(L"BackGround\\Mari_Sportswear.webm",30,L"jpg",95,true);
	//tutorial->m_layer = Define::Disable;
	//tutorial->SetRelativePosition(FVector2(-960, -550));

	auto PopupTab = m_owner->AddComponent<SpriteRenderer>();
	PopupTab->LoadData(L"UI\\UI_NewPopup.png");
	PopupTab->SetRelativeScale(1.f);
	PopupTab->SetLayer(Define::Disable);
	PopupTab->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f , -SCREEN_HEIGHT / 2.0f + 50));

	auto qrCodeSprite = m_owner->AddComponent<SpriteRenderer>();
	qrCodeSprite->LoadData(L"UI\\Github.jpg");

	auto githubGuideTeamSprite = m_owner->AddComponent<SpriteRenderer>();
	githubGuideTeamSprite->LoadData(L"UI\\Github_Team_Guide.png");

	if (!startText || !startButton) return;
	if (!continueText || !continueButton || !continueTabText) return;
	if (!optionText || !optionButton || !optionTabText) return;
	if (!staffText || !staffButton || !staffTabText) return;
	if (!quitText || !quitButton) return;
	//if (!spineShowText || !spineShowButton) return;
	if (!closeText || !closeButton) return;
	if (!sound || !PopupTab) return;

	float buttonBasePos = 270;

	// ======================== startButton
	//startButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	//FVector2 startButtonSize = startButton->GetRelativeSize();
	//startButton->SetRelativePosition(CoordHelper::RatioCoordToScreen(startButtonSize, FVector2(1, 0))
	//	+ FVector2(buttonBasePos, -150));
	//startButton->SetRelativeScale(FVector2(1, 1));
	//startButton->m_layer = Define::ButtonLayer;

	// ======================== continueButton
	continueButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_alpha.png");
	continueButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_alpha.png");
	continueButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_alpha.png");
	continueButton->LoadData(Define::EButtonState::Release, L"UI\\Button_alpha.png");
	FVector2 continueButtonSize = continueButton->GetRelativeSize();
	continueButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 130));
	continueButton->SetRelativeScale(FVector2(1, 1));
	continueButton->SetLayer(Define::ButtonLayer);

	// ======================== optionButton
	optionButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_alpha.png");
	optionButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_alpha.png");
	optionButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_alpha.png");
	optionButton->LoadData(Define::EButtonState::Release, L"UI\\Button_alpha.png");
	FVector2 optionButtonSize = optionButton->GetRelativeSize();
	optionButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 210));
	optionButton->SetRelativeScale(FVector2(1, 1));
	optionButton->SetLayer(Define::ButtonLayer);

	// ======================== staffButton
	staffButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_alpha.png");
	staffButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_alpha.png");
	staffButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_alpha.png");
	staffButton->LoadData(Define::EButtonState::Release, L"UI\\Button_alpha.png");
	FVector2 staffButtonSize = staffButton->GetRelativeSize();
	staffButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos + 30, 290));
	staffButton->SetRelativeScale(FVector2(1, 1));
	staffButton->SetLayer(Define::ButtonLayer);

	// ======================== quitButton
	quitButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_alpha.png");
	quitButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_alpha.png");
	quitButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_alpha.png");
	quitButton->LoadData(Define::EButtonState::Release, L"UI\\Button_alpha.png");
	FVector2 quitButtonSize = quitButton->GetRelativeSize();
	quitButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(quitButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 370));
	quitButton->SetRelativeScale(FVector2(1, 1));
	quitButton->SetRelativeRotation(0);
	quitButton->SetLayer(Define::ButtonLayer);

	// ======================== spineShowButton
	//spineShowButton->SetTag(L"Button");
	//spineShowButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	//spineShowButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	//spineShowButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	//spineShowButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	//FVector2 spineShowButtonSize = spineShowButton->GetRelativeSize();
	//spineShowButton->SetRelativePosition(
	//	CoordHelper::RatioCoordToScreen(spineShowButtonSize, FVector2(1, 0))
	//	+ FVector2(-1000, 450));
	//spineShowButton->SetRelativeScale(FVector2(1, 1));
	//spineShowButton->SetRelativeRotation(0);
	//spineShowButton->m_layer = Define::ButtonLayer;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	closeButton->SetRelativePosition(FVector2(0,350));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetActive(false);
			closeButton->SetLayer(Define::Disable);

	smallClose->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
    smallClose->SetDrawType(EDrawType::ScreenSpace);
    smallClose->SetRelativeScale(0.43f);
    smallClose->SetActive(false);
    smallClose->SetLayer(Define::Disable);
    // 패널 우상단 모서리 근처에 배치
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float hh = soundControl->GetBitmapSizeY() * soundUISize * 0.5f;
        const float margin = 20.f;
        smallClose->SetRelativePosition(FVector2(hw - margin, -hh + margin));
    }

	// ======================== artBookButton
	    artBookButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
    artBookButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
    artBookButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
    artBookButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
    artBookButton->SetRelativePosition(FVector2(-400, 0));
    artBookButton->SetRelativeScale(FVector2(1, 1));
    artBookButton->SetLayer(Define::Disable);

	// ======================== soundButton
	bgmPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	bgmPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 100.f;
		const float marginY = 82.5f;
        const float rowY = -panelH * 0.25f; // 위쪽 라인
        bgmPlusButton->SetRelativePosition(FVector2(hw - marginX, rowY + marginY));
    }
	    bgmPlusButton->SetActive(false);
    bgmPlusButton->SetLayer(Define::Disable);

	bgmMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	bgmMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 110.f;
		const float marginY = 82.5f;
        const float rowY = -panelH * 0.25f;
        bgmMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY + marginY));
    }
	    bgmMinusButton->SetActive(false);
    bgmMinusButton->SetLayer(Define::Disable);

	sfxPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	sfxPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 100.f;
		const float marginY = 11.0f;
        const float rowY = panelH * 0.25f; // 아래쪽 라인
        sfxPlusButton->SetRelativePosition(FVector2(hw - marginX, rowY - marginY));
    }
	    sfxPlusButton->SetActive(false);
    sfxPlusButton->SetLayer(Define::Disable);

	sfxMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	sfxMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 110.5f;
		const float marginY = 10.7f;
        const float rowY = panelH * 0.25f;
        sfxMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY - marginY));
    }
		sfxMinusButton->SetActive(false);
		sfxMinusButton->SetLayer(Define::Disable);

	// ======================== mainTitle
	//mainTitle->SetFontSize(120.0f);
	//mainTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//mainTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//mainTitle->SetText(L"높다락길의 질주");
	//mainTitle->SetColor(FColor(242, 207, 238, 255));
	//FVector2 mainTitleRectSize = mainTitle->GetRelativeSize();
	//mainTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, -300));
	//mainTitle->SetRelativeScale(FVector2(1, 1));
	//mainTitle->SetRelativeRotation(0);
	//mainTitle->m_layer = Define::NormalTextLayer;

	// ======================== subTitle
	//subTitle->SetFontSize(60.0f);
	//subTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//subTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//subTitle->SetText(L"~활빈은 두령을 쫒는다~");
	//subTitle->SetColor(FColor(242, 207, 238, 255));
	//FVector2 subTitleRectSize = subTitle->GetRelativeSize();
	//subTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(subTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, 0));
	//subTitle->SetRelativeScale(FVector2(1, 1));
	//subTitle->SetRelativeRotation(0);
	//subTitle->m_layer = Define::NormalTextLayer;

	// ======================== startText
	//startText->SetFontSize(55.0f);
	//startText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//startText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//startText->SetText(L"시작하기");
	//startText->SetColor(FColor::White);
	//FVector2 startTextRectSize = startText->GetRelativeSize();
	//startText->SetRelativePosition(CoordHelper::RatioCoordToScreen(startTextRectSize, FVector2(-0.5, -0.5)));
	//startText->SetRelativeScale(FVector2(1, 1));
	//startText->SetRelativeRotation(0);
	//startText->m_layer = Define::ButtonTextLayer;
	//startText->RemoveFromParent();
	//startButton->AddChildComponent(startText);

	// ======================== continueText
	continueText->SetFontSize(55.0f);
	continueText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	continueText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	continueText->SetText(L"임무 선택");
	continueText->SetColor(FColor(0, 234, 255, 255));
	FVector2 continueTextRectSize = continueText->GetRelativeSize();
	continueText->SetRelativePosition(CoordHelper::RatioCoordToScreen(continueTextRectSize, FVector2(-0.5, -0.5)));
	continueText->SetRelativeScale(FVector2(1, 1));
	continueText->SetRelativeRotation(0);
	continueText->SetLayer(Define::ButtonTextLayer);
	continueText->RemoveFromParent();
	continueButton->AddChildComponent(continueText);

	//continueTabText->SetFontSize(70.0f);
	//continueTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//continueTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//continueTabText->SetText(L"임무 선택");
	//continueTabText->SetColor(FColor(0, 234, 255, 255));
	//FVector2 continueTabTextRectSize = continueTabText->GetRelativeSize();
	//continueTabText->SetRelativePosition(
	//	CoordHelper::RatioCoordToScreen(continueTabTextRectSize, FVector2(-0.5, -0.5))
	//	+ FVector2(0, -350)
	//);
	//continueTabText->SetRelativeRotation(0);
	//continueTabText->m_layer = -1000;

	// ======================== optionText
	optionText->SetFontSize(55.0f);
	optionText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionText->SetText(L"음량 조절");
	optionText->SetColor(FColor(0, 234, 255, 255));
	FVector2 optionTextRectSize = optionText->GetRelativeSize();
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionTextRectSize, FVector2(-0.5, -0.5)));
	optionText->SetRelativeScale(FVector2(1, 1));
	optionText->SetRelativeRotation(0);
	optionText->SetLayer(Define::ButtonTextLayer);
	optionText->RemoveFromParent();
	optionButton->AddChildComponent(optionText);

	optionTabText->SetFontSize(55.0f);
	optionTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabText->SetText(L"음량 조절");
	optionTabText->SetColor(FColor::White);
	FVector2 optionTabTextRectSize = optionTabText->GetRelativeSize();
	optionTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -85)
	);
	optionTabText->SetRelativeRotation(0);
	optionTabText->SetLayer(Define::Disable);

	optionTabBGMText->SetFontSize(20.0f);
	optionTabBGMText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabBGMText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabBGMText->SetText(L"배경음");
	optionTabBGMText->SetColor(FColor(0, 234, 255, 255));
	optionTabBGMText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabBGMText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, -30)
	);
	optionTabBGMText->SetRelativeRotation(0);
	optionTabBGMText->SetLayer(Define::Disable);

	optionTabSFXText->SetFontSize(20.0f);
	optionTabSFXText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabSFXText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabSFXText->SetText(L"효과음");
	optionTabSFXText->SetColor(FColor(0, 234, 255, 255));
	optionTabSFXText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabSFXText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 35)
	);
	optionTabSFXText->SetRelativeRotation(0);
	optionTabSFXText->SetLayer(Define::Disable);

	// ======================== staffText
	staffText->SetFontSize(55.0f);
	staffText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffText->SetText(L"제작진");
	staffText->SetColor(FColor(0, 234, 255, 255));
	FVector2 staffTextRectSize = continueText->GetRelativeSize();
	staffText->SetRelativePosition(CoordHelper::RatioCoordToScreen(staffTextRectSize, FVector2(-0.5, -0.5)) + FVector2(20,0));
	staffText->SetRelativeScale(FVector2(1, 1));
	staffText->SetRelativeRotation(0);
	staffText->SetLayer(Define::ButtonTextLayer);
	staffText->RemoveFromParent();
	staffButton->AddChildComponent(staffText);

	staffTabText->SetFontSize(70.0f);
	staffTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffTabText->SetText(L"제작진");
	staffTabText->SetColor(FColor(0, 234, 255, 255));
	FVector2 staffTabTextSize = staffTabText->GetRelativeSize();
	staffTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffTabTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -300)
	);
	staffTabText->SetRelativeRotation(0);
	staffTabText->SetLayer(Define::Disable);

	staffNameText->SetFontSize(55.0f);
	staffNameText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffNameText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffNameText->SetText(
		L"기획\n"
		L"이유성 | 신소영\n"
		L"\n"
		L"아트\n"
		L"강연주 | 민지인\n"
		L"\n"
		L"프로그래밍\n"
		L"이창진 | 강성근 | 황태현\n"
	);
	staffNameText->SetColor(FColor::White);
	FVector2 staffNameTextSize = staffNameText->GetRelativeSize();
	staffNameText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffNameTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, 50)
	);
	staffNameText->SetLayer(Define::Disable);

	qrCodeSprite->SetDrawType(Define::EDrawType::ScreenSpace);
	FVector2 qrCodeSpriteSize = qrCodeSprite->GetRelativeSize();
	qrCodeSprite->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(qrCodeSpriteSize, FVector2(-0.5, -0.5))
		+ FVector2(500, 350)
	);
	qrCodeSprite->SetRelativeScale(0.6);
	qrCodeSprite->SetLayer(Define::Disable);

	githubGuideTeamSprite->SetDrawType(Define::EDrawType::ScreenSpace);
	githubGuideTeamSprite->SetRelativeScale(FVector2(0.5,0.57));
	FVector2 githubGuideTeamSpriteSize = githubGuideTeamSprite->GetRelativeSize();
	githubGuideTeamSprite->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(githubGuideTeamSpriteSize, FVector2(-0.5, -0.5))
		+ FVector2(-250, 260)
	);
	githubGuideTeamSprite->SetLayer(Define::Disable);

	// ======================== quitText
	quitText->SetFontSize(55.0f);
	quitText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	quitText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	quitText->SetText(L"오락 종료");
	quitText->SetColor(FColor(0, 234, 255, 255));
	FVector2 quitTextRectSize = quitText->GetRelativeSize();
	quitText->SetRelativePosition(CoordHelper::RatioCoordToScreen(quitTextRectSize, FVector2(-0.5, -0.5)));
	quitText->SetRelativeScale(FVector2(1, 1));
	quitText->SetRelativeRotation(0);
	quitText->SetLayer(Define::ButtonTextLayer);
	quitText->RemoveFromParent();
	quitButton->AddChildComponent(quitText);

	// ======================== spineShowText
	//spineShowText->SetFontSize(43.0f);
	//spineShowText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//spineShowText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//spineShowText->SetText(L"Spine2D 보러가기");
	//spineShowText->SetColor(FColor::White);
	//FVector2 spineShowTextRectSize = spineShowText->GetRelativeSize();
	//spineShowText->SetRelativePosition(CoordHelper::RatioCoordToScreen(spineShowTextRectSize, FVector2(-0.5, -0.5)));
	//spineShowText->SetRelativeScale(FVector2(1, 1));
	//spineShowText->SetRelativeRotation(0);
	//spineShowText->m_layer = Define::ButtonTextLayer;
	//spineShowText->RemoveFromParent();
	//spineShowButton->AddChildComponent(spineShowText);

	// ======================== closeText
	closeText->SetFontSize(55.0f);
	closeText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	closeText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	closeText->SetText(L"닫기");
	closeText->SetColor(FColor::White);
	FVector2 closeTextRectSize = closeText->GetRelativeSize();
	closeText->SetRelativePosition(CoordHelper::RatioCoordToScreen(closeTextRectSize, FVector2(-0.5, -0.5)));
	closeText->SetRelativeScale(FVector2(1, 1));
	closeText->SetRelativeRotation(0);
	closeText->SetLayer(Define::Disable);
	closeButton->SetActive(false);
	closeText->RemoveFromParent();
	closeButton->AddChildComponent(closeText);

	// ======================== skipText
	artBookText->SetFontSize(55.0f);
	artBookText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	artBookText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	artBookText->SetText(L"디지털 아트북");
	artBookText->SetColor(FColor::White);
	FVector2 skipTextRectSize = artBookText->GetRelativeSize();
	artBookText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextRectSize, FVector2(-0.5, -0.5)));
	artBookText->SetRelativeScale(FVector2(1, 1));
	artBookText->SetRelativeRotation(0);
	artBookText->SetLayer(Define::Disable);
	artBookButton->SetActive(true);
	artBookText->RemoveFromParent();
	artBookButton->AddChildComponent(artBookText);

	// ======================== background
	background->LoadData(L"UI/MAIN_02.png");
	background->SetDrawType(Define::EDrawType::ScreenSpace);
	FVector2 backgroundSize = background->GetRelativeSize();
	background->SetRelativePosition(CoordHelper::RatioCoordToScreen(backgroundSize, FVector2(0, 0)));
	background->SetRelativeScale(FVector2(1, 1));
	background->SetRelativeRotation(0);
	background->SetLayer(0);

	textLogo->LoadData(L"UI\\LOGO.png");
	textLogo->SetDrawType(EDrawType::ScreenSpace);
	textLogo->SetFilter(FColor(0, 234, 255, 255));
	textLogo->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(textLogo->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(470, -50)
	);
	textLogo->SetRelativeScale(FVector2(1, 1));
	textLogo->SetRelativeRotation(0);
	textLogo->SetLayer(Define::NormalTextLayer);

	// ======================== Delegete
	/*startButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, quitButton, staffButton, optionButton, closeButton, closeText,
		sound, continueButton, artBookButton, artBookText
	]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		uiSound->StopByName(L"UISound");
		uiSound->PlayByName1(L"UISound", 0.45f);

		 다른 버튼 비활성화 (애니메이션 자동 중지됨)
		startButton->SetActive(false);
		quitButton->SetActive(false);
		staffButton->SetActive(false);
		optionButton->SetActive(false);
		continueButton->SetActive(false);

		closeButton->SetLayer(503);
		closeButton->SetActive(true);
		closeText->SetLayer(504);

		artBookButton->SetLayer(Define::PopupButtonLayer);
		artBookButton->SetActive(true);
		artBookText->SetLayer(Define::PopupTextLayer);

		tutorial->SetLayer(Define::PopupLayer);
		tutorial->Play();
	});*/
	artBookButton->SetStateAction(Define::EButtonState::Hover, [artBookButton]()
		{
			artBookButton->StartHoverPulse(0.8f, 0.04f);
			artBookButton->StartEffectAnimation(0.3f, 1.2f, FColor(64, 224, 208, 255));
		});

	artBookButton->SetStateAction(Define::EButtonState::HoverLeave, [artBookButton]()
		{
			artBookButton->StopHoverPulse();
			artBookButton->StartEffectAnimation(0.2f, 0.0f, FColor(64, 224, 208, 255));
		});

	artBookButton->SetStateAction(Define::EButtonState::Release, [artBookButton]()
		{
			artBookButton->StopHoverPulse();
			artBookButton->StartEffectAnimation(0.1f, 0.0f, FColor(64, 224, 208, 255));
		});

	// TODO : 씬전환
	artBookButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(Define::Scene_Stage1);
		});

	continueButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText, PopupTab,
		sound, continueTabText
	]()		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			//uiSound->StopByName(L"UISound");
			//uiSound->PlayByName1(L"UISound", 0.45f);
			
			// 여기에 몇 초 뒤 씬 전환 넣으면 될 거 같음
			SceneManager::ChangeScene(L"SelectScene");

			// 다른 버튼 비활성화
			//startButton->SetActive(false);
			//continueButton->SetActive(false);
			//quitButton->SetActive(false);
			//staffButton->SetActive(false);
			//optionButton->SetActive(false);

			//closeButton->m_layer = 503;
			//closeButton->SetActive(true);
			//closeText->m_layer = 504;

			//continueTabText->m_layer = 503;

							//PopupTab->SetLayer(502);
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText , PopupTab,
		sound, continueTabText, optionTabText, staffTabText, staffNameText, qrCodeSprite, githubGuideTeamSprite,
		artBookButton, artBookText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			//uiSound->StopByName(L"UISound");
			//uiSound->PlayByName1(L"UISound", 0.45f);

			// 모두 활성화
			startButton->SetActive(true);
			continueButton->SetActive(true);
			quitButton->SetActive(true);
			//spineShowButton->SetActive(true);
			staffButton->SetActive(true);
			optionButton->SetActive(true);

			closeButton->SetLayer(Define::Disable);
			closeButton->SetActive(false);
			closeText->SetLayer(Define::Disable);

			continueTabText->SetLayer(Define::Disable);
			//optionTabText->m_layer = -1000;
			staffTabText->SetLayer(Define::Disable);
			staffNameText->SetLayer(Define::Disable);
			qrCodeSprite->SetLayer(Define::Disable);
			githubGuideTeamSprite->SetLayer(Define::Disable);
			artBookButton->SetLayer(Define::Disable);
			artBookText->SetLayer(Define::Disable);

			//tutorial->m_layer = Define::Disable;
			//tutorial->Stop();

			PopupTab->SetLayer(Define::Disable);
		});

	smallClose->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, smallClose,
		bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl, sound, optionTabText,
		optionTabBGMText, optionTabSFXText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		startButton->SetActive(true);
		continueButton->SetActive(true);
		quitButton->SetActive(true);
		//spineShowButton->SetActive(true);
		staffButton->SetActive(true);
		optionButton->SetActive(true);

		smallClose->SetLayer(Define::Disable);
		smallClose->SetActive(false);

		bgmMinusButton->SetActive(false);
		bgmPlusButton->SetActive(false);
		bgmMinusButton->SetLayer(Define::Disable);
		bgmPlusButton->SetLayer(Define::Disable);

		sfxPlusButton->SetActive(false);
		sfxMinusButton->SetActive(false);
		sfxPlusButton->SetLayer(Define::Disable);
		sfxMinusButton->SetLayer(Define::Disable);

			optionTabText->SetLayer(Define::Disable);
			optionTabBGMText->SetLayer(Define::Disable);
			optionTabSFXText->SetLayer(Define::Disable);

			sfxControl->SetLayer(Define::Disable);
			bgmControl->SetLayer(Define::Disable);
			soundControl->SetLayer(Define::Disable);
		});

	optionButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab,
		sound, optionTabText, bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl, smallClose, optionTabBGMText, optionTabSFXText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			
			//if (uiSound->IsPlaying())
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			// 다른 버튼 비활성화 (애니메이션 자동 중지됨)
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			//spineShowButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);
			
			smallClose->SetActive(true);
			smallClose->SetLayer(Define::PopupButtonLayer);

			//optionTabText->m_layer = 503;
			bgmMinusButton->SetActive(true);
			bgmPlusButton->SetActive(true);
			bgmMinusButton->SetLayer(Define::PopupButtonLayer);
			bgmPlusButton->SetLayer(Define::PopupButtonLayer);

			sfxPlusButton->SetActive(true);
			sfxMinusButton->SetActive(true);
			sfxPlusButton->SetLayer(Define::PopupButtonLayer);
			sfxMinusButton->SetLayer(Define::PopupButtonLayer);

			optionTabText->SetLayer(Define::PopupTextLayer);
			optionTabBGMText->SetLayer(Define::PopupTextLayer);
			optionTabSFXText->SetLayer(Define::PopupTextLayer);

			sfxControl->SetLayer(Define::PopupObjectLayer);
			bgmControl->SetLayer(Define::PopupObjectLayer);
			soundControl->SetLayer(Define::PopupLayer);
		});

	staffButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab, closeButton, closeText,
		sound, staffTabText, staffNameText, qrCodeSprite, githubGuideTeamSprite, artBookButton, artBookText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			//if (uiSound->IsPlaying())
			//uiSound->StopByName(L"UISound");

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			// 다른 버튼 비활성화 (애니메이션 자동 중지됨)
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			//spineShowButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);

			closeButton->SetLayer(Define::PopupButtonLayer);
			closeButton->SetActive(true);
			closeText->SetLayer(Define::PopupTextLayer);

			staffTabText->SetLayer(Define::PopupTextLayer);
			staffNameText->SetLayer(Define::PopupTextLayer);
			qrCodeSprite->SetLayer(Define::PopupTextLayer);
			githubGuideTeamSprite->SetLayer(Define::PopupTextLayer);
			artBookButton->SetLayer(Define::PopupButtonLayer);
			artBookText->SetLayer(Define::PopupTextLayer);

			PopupTab->SetLayer(Define::PopupLayer);
		});

	// bgmVolume
    bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl]
		{
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetBGMVolume();
			vol -= 0.1f;
			AudioManager::GetInstance().SetBGMVolume(vol);

			bgmControl->SetProgress(vol);
		});

    bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl]
		{
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetBGMVolume();
			vol += 0.1f;
			AudioManager::GetInstance().SetBGMVolume(vol);

			bgmControl->SetProgress(vol);
		});

    sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [
        sound, sfxControl
    ] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetSFXVolume();
			vol -= 0.1f;
			AudioManager::GetInstance().SetSFXVolume(vol);

			sfxControl->SetProgress(vol);
		});


    sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [
        sound, sfxControl
    ] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");
		
			float vol = AudioManager::GetInstance().GetSFXVolume();
			vol += 0.1f;
			AudioManager::GetInstance().SetSFXVolume(vol);

			sfxControl->SetProgress(vol);
		});


	quitButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());

			// Quit
			//PostQuitMessage(0);
			Application::GetInstance().GameQuit();
		});

	quitButton->SetStateAction(Define::EButtonState::Hover, [quitButton, quitText]()
	{
		quitButton->StartHoverPulse(0.8f, 0.04f);
		quitText->SetColor(FColor::White);
	});

	quitButton->SetStateAction(Define::EButtonState::HoverLeave, [quitButton, quitText]()
	{
		quitButton->StopHoverPulse();
		quitText->SetColor(FColor(0, 234, 255, 255));
	});

	quitButton->SetStateAction(Define::EButtonState::Release, [quitButton, quitText]()
	{
		quitButton->StopHoverPulse();
		quitText->SetColor(FColor(0, 234, 255, 255));
	});

	//spineShowButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	//	{
	//		OutputDebugStringW(L"SetAction click!\n");
	//		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
	//
	//		// Quit
	//		//PostQuitMessage(0);
	//		//Application::GetInstance().GameQuit();
	//		if (m_spineRender)
	//		{
	//			spine->spineRenderer->SetRendered(false);
	//			spine2->spineRenderer->SetRendered(false);
	//			m_spineRender = false;
	//		}
	//		else
	//		{
	//			spine->spineRenderer->SetRendered(true);
	//			spine2->spineRenderer->SetRendered(true);
	//			m_spineRender = true;
	//		}
	//		
	//	});
	//
	//spineShowButton->SetStateAction(Define::EButtonState::Hover, [spineShowButton]()
	//	{
	//		spineShowButton->StartHoverPulse(0.8f, 0.04f);
	//		spineShowButton->StartEffectAnimation(0.3f, 1.2f, FColor::White);
	//	});
	//
	//spineShowButton->SetStateAction(Define::EButtonState::HoverLeave, [spineShowButton]()
	//	{
	//		spineShowButton->StopHoverPulse();
	//		spineShowButton->StartEffectAnimation(0.2f, 0.0f, FColor::White);
	//	});
	//
	//spineShowButton->SetStateAction(Define::EButtonState::Release, [spineShowButton]()
	//	{
	//		spineShowButton->StopHoverPulse();
	//		spineShowButton->StartEffectAnimation(0.1f, 0.0f, FColor::White);
	//	});

	optionButton->SetStateAction(Define::EButtonState::Hover, [optionButton, optionText]()
	{
		optionButton->StartHoverPulse(0.8f, 0.04f);
		optionText->SetColor(FColor::White);
	});

	optionButton->SetStateAction(Define::EButtonState::HoverLeave, [optionButton, optionText]()
	{
		optionButton->StopHoverPulse();
		optionText->SetColor(FColor(0, 234, 255, 255));
	});

	optionButton->SetStateAction(Define::EButtonState::Release, [optionButton, optionText]()
	{
		optionButton->StopHoverPulse();
		optionText->SetColor(FColor(0, 234, 255, 255));
	});

	staffButton->SetStateAction(Define::EButtonState::Hover, [staffButton, staffText]()
	{
		staffButton->StartHoverPulse(0.8f, 0.04f);
		staffText->SetColor(FColor::White);
	});

	staffButton->SetStateAction(Define::EButtonState::HoverLeave, [staffButton, staffText]()
	{
		staffButton->StopHoverPulse();
		staffText->SetColor(FColor(0, 234, 255, 255));
	});

	staffButton->SetStateAction(Define::EButtonState::Release, [staffButton, staffText]()
	{
		staffButton->StopHoverPulse();
		staffText->SetColor(FColor(0, 234, 255, 255));
	});

	continueButton->SetStateAction(Define::EButtonState::Hover, [continueButton, continueText]()
	{
		continueButton->StartHoverPulse(0.8f, 0.04f);
		continueText->SetColor(FColor::White);
	});

	continueButton->SetStateAction(Define::EButtonState::HoverLeave, [continueButton, continueText]()
	{
		continueButton->StopHoverPulse();
		continueText->SetColor(FColor(0, 234, 255, 255));
	});

	continueButton->SetStateAction(Define::EButtonState::Release, [continueButton, continueText]()
	{
		continueButton->StopHoverPulse();
		continueText->SetColor(FColor(0, 234, 255, 255));
	});

	// closeButton Hover 효과 (주황색 글로우)
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

	// bgmPlusButton Hover 효과 (시안색 글로우)
	bgmPlusButton->SetStateAction(Define::EButtonState::Hover, [bgmPlusButton]()
	{
		bgmPlusButton->StartHoverPulse(0.8f, 0.04f);
		bgmPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Cyan);
	});

	bgmPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmPlusButton]()
	{
		bgmPlusButton->StopHoverPulse();
		bgmPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Cyan);
	});

	bgmPlusButton->SetStateAction(Define::EButtonState::Release, [bgmPlusButton]()
	{
		bgmPlusButton->StopHoverPulse();
		bgmPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Cyan);
	});

	// bgmMinusButton Hover 효과 (마젠타색 글로우)
	bgmMinusButton->SetStateAction(Define::EButtonState::Hover, [bgmMinusButton]()
	{
		bgmMinusButton->StartHoverPulse(0.8f, 0.04f);
		bgmMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Magenta);
	});

	bgmMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmMinusButton]()
	{
		bgmMinusButton->StopHoverPulse();
		bgmMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Magenta);
	});

	bgmMinusButton->SetStateAction(Define::EButtonState::Release, [bgmMinusButton]()
	{
		bgmMinusButton->StopHoverPulse();
		bgmMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Magenta);
	});

	// sfxPlusButton Hover 효과 (금색 글로우)
	sfxPlusButton->SetStateAction(Define::EButtonState::Hover, [sfxPlusButton]()
	{
		sfxPlusButton->StartHoverPulse(0.8f, 0.04f);
		sfxPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Gold);
	});

	sfxPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxPlusButton]()
	{
		sfxPlusButton->StopHoverPulse();
		sfxPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Gold);
	});

	sfxPlusButton->SetStateAction(Define::EButtonState::Release, [sfxPlusButton]()
	{
		sfxPlusButton->StopHoverPulse();
		sfxPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Gold);
	});

	// sfxMinusButton Hover 효과 (분홍색 글로우)
	sfxMinusButton->SetStateAction(Define::EButtonState::Hover, [sfxMinusButton]()
	{
		sfxMinusButton->StartHoverPulse(0.8f, 0.04f);
		sfxMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Pink);
	});

	sfxMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxMinusButton]()
	{
		sfxMinusButton->StopHoverPulse();
		sfxMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Pink);
	});

	sfxMinusButton->SetStateAction(Define::EButtonState::Release, [sfxMinusButton]()
	{
		sfxMinusButton->StopHoverPulse();
		sfxMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Pink);
	});

	// startButton Hover 효과 (펄스 + 글로우 이펙트)
	startButton->SetStateAction(Define::EButtonState::Hover, [startButton]()
	{
		startButton->StartHoverPulse(0.8f, 0.04f);
		// 빨간색 글로우 - 훨씬 간단해짐!
		startButton->StartEffectAnimation(0.3f, 1.2f, FColor::Red);
	});

	startButton->SetStateAction(Define::EButtonState::HoverLeave, [startButton]()
	{
		startButton->StopHoverPulse();
		startButton->StartEffectAnimation(0.2f, 0.0f, FColor::Red);
	});

	startButton->SetStateAction(Define::EButtonState::Release, [startButton]()
	{
		startButton->StopHoverPulse();
		startButton->StartEffectAnimation(0.1f, 0.0f, FColor::Red); 
	});
}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	
}

void TitleWidgetScript::OnDestroy()
{

}
