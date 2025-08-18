#include "StageWidgetScript.h"

#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Math/Transform.h>
#include <Helpers/CoordHelper.h>

#include <Core/Input.h>
#include <Scene/Scene.h>
#include <Component/SpriteRenderer.h>
#include <Component/TextRenderComponent.h>
#include <Component/ButtonComponent.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>

#include <GameManager/GamePlayManager.h>
#include <Component/ProgressBarComponent.h>

#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include "../Audio/StageAudioScript.h"
#include <Component/InputComponent.h>

void StageWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void StageWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	
	//GamePlayManager& gm = GamePlayManager::GetInstance();

	float timeSec = GamePlayManager::GetInstance().GetPassedTime();

	int minutes = static_cast<int>(timeSec) / 60;
	int seconds = static_cast<int>(timeSec) % 60;
	int milliseconds = static_cast<int>((timeSec - static_cast<int>(timeSec)) * 100.0f);

	wchar_t buffer[16];
	swprintf(buffer, 16, L"%02d:%02d:%02d", minutes, seconds, milliseconds);

	m_passedTimeText->SetText(std::wstring(buffer));

	//m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));

	// 배터리 계산
	int killCount = GamePlayManager::GetInstance().GetKillEnemyAmount();
	int& batteryCount = GamePlayManager::GetInstance().batteryCount;

	if (prevKillAmount != killCount) {
		int delta = killCount - prevKillAmount;

		batteryCount = batteryCount + delta > maxBattery ?
			maxBattery : batteryCount + delta;

		prevKillAmount = killCount;
	}

	// 배터리 프로그래스 설정
	if (prevBattery != batteryCount) {
		SetProgress();
		prevBattery = batteryCount;
	}

	// =======================================

	// 속도 연동
	float playerSpeed = 0.0f;
	if (WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player"))
	{
		if (BikeMovementScript* pm = player->GetComponent<BikeMovementScript>())
			playerSpeed = pm->GetCurrentSpeed();// * pm->GetSpeedModifierValue();
	}

	float progress = playerSpeed / 600.0f;

	if (progress > 1.0f) progress = 0.999999f;	// 어느정도 허용치를 둠

	m_speedProgress->SetProgress(progress);

	int maxSpeed = static_cast<int>(std::round(playerSpeed / 3.0f));
	if (maxSpeed > 200) maxSpeed = 200;

	wchar_t speedBuffer[8];
	swprintf(speedBuffer, 8, L"%03d", maxSpeed);

	m_speedText->SetText(std::wstring(speedBuffer));


	// =====================================

	static bool wasPlaying = true; // 초기값은 컷씬 중이라고 가정

	bool isPlaying = GamePlayManager::GetInstance().IsCutScenePlaying();

	if (wasPlaying != isPlaying)
	{
		m_pauseButton->SetActive(!isPlaying);
		wasPlaying = isPlaying;
	}

	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	auto sound = bgmObj->GetComponent<AudioComponent>();

	if (!GamePlayManager::GetInstance().IsCutScenePlaying())
	{
		if (!m_ambiencePlayed)
		{
			sound->PlayByName(L"Ambience");
			m_ambiencePlayed = true; // 다음부터는 안 들어오게
		}
	}
	else
	{
		m_ambiencePlayed = false; // 컷씬이 다시 시작하면 초기화
	}
}

void StageWidgetScript::Awake()
{
	// 씬 처음 들어오면 배터리 카운트는 0
	GamePlayManager::GetInstance().batteryCount = 0;
}

void StageWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	float soundUISize = 1;
	float soundTabPosX = 450;

	auto pauseButton = m_owner->AddComponent<ButtonComponent>();
	m_pauseButton = pauseButton;
	auto closeButton = m_owner->AddComponent<ButtonComponent>();
	auto smallClose = m_owner->AddComponent<ButtonComponent>();

	auto toOption = m_owner->AddComponent<ButtonComponent>();
	auto toMain = m_owner->AddComponent<ButtonComponent>();
	auto toRestart = m_owner->AddComponent<ButtonComponent>();
	auto toSelect = m_owner->AddComponent<ButtonComponent>();

	auto bgmPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto bgmMinusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxMinusButton = m_owner->AddComponent<ButtonComponent>();

	auto pauseText = m_owner->AddComponent<TextRenderComponent>();
	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto mainText = m_owner->AddComponent<TextRenderComponent>();
	auto restartText = m_owner->AddComponent<TextRenderComponent>();
	auto selectText = m_owner->AddComponent<TextRenderComponent>();
	auto velocityText = m_owner->AddComponent<TextRenderComponent>();

	auto optionTabText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabBGMText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabSFXText = m_owner->AddComponent<TextRenderComponent>();

	auto UI_Timer = m_owner->AddComponent<SpriteRenderer>();
	auto UI_HP = m_owner->AddComponent<SpriteRenderer>();
	auto UI_Dashboard = m_owner->AddComponent<SpriteRenderer>();

	m_batteryProgress = m_owner->AddComponent<ProgressBarComponent>();
	m_speedProgress = m_owner->AddComponent<ProgressBarComponent>();
	auto bgmControl = m_owner->AddComponent<ProgressBarComponent>();
	auto sfxControl = m_owner->AddComponent<ProgressBarComponent>();

	m_dashboardText = m_owner->AddComponent<TextRenderComponent>();
	m_speedText = m_owner->AddComponent<TextRenderComponent>();

	// ================== Sprite
	auto popUpTab = m_owner->AddComponent<SpriteRenderer>();
	popUpTab->SetDrawType(EDrawType::ScreenSpace);
	//popUpTab->LoadData(L"UI\\UI_PauseTab.png");
	popUpTab->LoadData(L"UI\\UI_Tab.png");	// 텍스트를 넣읗거면 이거로 사용
	popUpTab->SetRelativePosition(FVector2(0, 0));
	popUpTab->SetLayer(Define::Disable);
	popUpTab->SetOpacity(0);

	UI_Timer->LoadData(L"UI\\UI_Time.png");
	UI_Timer->SetDrawType(EDrawType::ScreenSpace);
	FVector2 TimerSize = UI_Timer->GetRelativeSize();
	UI_Timer->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(TimerSize, FVector2(0, 0))
		+ FVector2(-820, -SCREEN_HEIGHT / 2.0f + 80));
	UI_Timer->SetLayer(Define::HUDLayer);

	UI_Dashboard->LoadData(L"UI\\UI_Dashboard.png");
	UI_Dashboard->SetDrawType(EDrawType::ScreenSpace);
	FVector2 DashboardSize = UI_Dashboard->GetRelativeSize();
	UI_Dashboard->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(DashboardSize, FVector2(0, 0))
		+ FVector2(-670, SCREEN_HEIGHT / 2.0f - 210)
	);
	UI_Dashboard->SetLayer(Define::HUDLayer);

	// Progress bar
	m_batteryProgress->SetDrawType(Define::EDrawType::ScreenSpace);
	m_batteryProgress->LoadData(L"UI\\UI_2_Battery.png");
	m_batteryProgress->SetType(EProgressBarType::Linear);
	m_batteryProgress->SetRelativeScale(FVector2(1, 1));
	m_batteryProgress->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_batteryProgress->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(-596, SCREEN_HEIGHT / 2.0f - 169));
	m_batteryProgress->SetProgress(0);
	m_batteryProgress->SetLayer(Define::HUDLayer + 10);

	// Progress bar - (원형 프로그레스)
	m_speedProgress->SetDrawType(Define::EDrawType::ScreenSpace);
	m_speedProgress->LoadData(L"UI\\UI_2_Speed.png");
	m_speedProgress->SetType(EProgressBarType::Radial);
	m_speedProgress->SetRelativeScale(FVector2(1, 1));
	m_speedProgress->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_speedProgress->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(-SCREEN_WIDTH / 2.0f + 139, SCREEN_HEIGHT / 2.0f - 210));
	// 초기값 - 최저속도일때 4칸
	m_speedProgress->SetStartAngleDeg(74);
	m_speedProgress->SetClockwise(true);
	m_speedProgress->SetProgress(1);
	m_speedProgress->SetLayer(Define::HUDLayer + 10);

	// 사운드 관련
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
	soundControl->SetRelativePosition(FVector2(soundTabPosX, 0.f));

	// Progress bar
	bgmControl->SetDrawType(Define::EDrawType::ScreenSpace);
	bgmControl->LoadData(L"UI\\ControlBar.png");
	bgmControl->SetType(EProgressBarType::Linear);
	bgmControl->SetRelativeScale(FVector2(1, 1));
	bgmControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(bgmControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(soundTabPosX + 5, 3));
	bgmControl->SetProgress(bgmVolume);
	bgmControl->SetLayer(Define::Disable);

	sfxControl->SetDrawType(Define::EDrawType::ScreenSpace);
	sfxControl->LoadData(L"UI\\ControlBar.png");
	sfxControl->SetType(EProgressBarType::Linear);
	sfxControl->SetRelativeScale(FVector2(1, 1));
	sfxControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(sfxControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(soundTabPosX + 5, 66));
	sfxControl->SetProgress(sfxVolume);
	sfxControl->SetLayer(Define::Disable);

	pauseText->SetFontSize(85.f);
	pauseText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	pauseText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	pauseText->SetText(L"일시정지");
	pauseText->SetColor(FColor::White);
	pauseText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(pauseText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, -200)
	);
	pauseText->SetLayer(Define::Disable);

	optionText->SetFontSize(40.f);
	optionText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionText->SetText(L"음량 조절");
	optionText->SetColor(FColor::White);
	optionText->RemoveFromParent();
	toOption->AddChildComponent(optionText);
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	optionText->SetLayer(Define::Disable);

	optionTabText->SetFontSize(55.0f);
	optionTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabText->SetText(L"음량 조절");
	optionTabText->SetColor(FColor::White);
	FVector2 optionTabTextRectSize = optionTabText->GetRelativeSize();
	optionTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(soundTabPosX, -85)
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
		+ FVector2(soundTabPosX, -30)
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
		+ FVector2(soundTabPosX, 35)
	);
	optionTabSFXText->SetRelativeRotation(0);
	optionTabSFXText->SetLayer(Define::Disable);

	mainText->SetFontSize(40.f);
	mainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	mainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	mainText->SetText(L"처음 화면으로");
	mainText->SetColor(FColor::White);
	mainText->RemoveFromParent();
	toMain->AddChildComponent(mainText);
	mainText->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	mainText->SetLayer(Define::Disable);

	restartText->SetFontSize(40.f);
	restartText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	restartText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	restartText->SetText(L"임무 재시작");
	restartText->SetColor(FColor::White);
	restartText->RemoveFromParent();
	toRestart->AddChildComponent(restartText);
	restartText->SetRelativePosition(CoordHelper::RatioCoordToScreen(restartText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	restartText->SetLayer(Define::Disable);

	selectText->SetFontSize(40.f);
	selectText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	selectText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	selectText->SetText(L"임무 선택화면");
	selectText->SetColor(FColor::White);
	selectText->RemoveFromParent();
	toSelect->AddChildComponent(selectText);
	selectText->SetRelativePosition(CoordHelper::RatioCoordToScreen(selectText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	selectText->SetLayer(Define::Disable);

	m_speedText->SetFontSize(40.f);
	m_speedText->SetFontFromFile(L"Fonts\\digital.ttf");
	m_speedText->SetFont(L"Digital-7 Mono", L"en-US");
	m_speedText->SetColor(FColor(0, 234, 255, 255));
	m_speedText->SetDrawType(EDrawType::ScreenSpace);
	m_speedText->SetLayer(Define::NormalTextLayer);
	m_speedText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_speedText->GetRelativeSize(), FVector2(-0.5, -0.5))
	+ FVector2(-865,330)
	);

	velocityText->SetFontSize(15.f);
	velocityText->SetFontFromFile(L"Fonts\\digital.ttf");
	velocityText->SetFont(L"Digital-7 Mono", L"en-US");
	velocityText->SetText(L"km/h");
	velocityText->SetColor(FColor(0, 234, 255, 255));
	velocityText->SetDrawType(EDrawType::ScreenSpace);
	velocityText->SetLayer(Define::NormalTextLayer);
	velocityText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(velocityText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(-795, 335)
	);

	// ========================== pauseButton
	pauseButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Release, L"UI\\UI_Pause.png");
	FVector2 pauseSize = pauseButton->GetRelativeSize();
	pauseButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(pauseSize, FVector2(0, 0))
				+ FVector2(900, -SCREEN_HEIGHT / 2.0f + 60)
		);
		pauseButton->SetLayer(Define::ButtonLayer);

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	closeButton->SetDrawType(EDrawType::ScreenSpace);
	closeButton->SetRelativePosition(FVector2(235, -345));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetActive(false);
	closeButton->SetLayer(Define::Disable);

	smallClose->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	smallClose->SetDrawType(EDrawType::ScreenSpace);
	smallClose->SetRelativePosition(FVector2(680, -130));
	smallClose->SetRelativeScale(FVector2(0.5, 0.5));
	smallClose->SetActive(false);
	smallClose->SetLayer(Define::Disable);

	// ======================== toRestart
	toRestart->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toRestart->SetDrawType(EDrawType::ScreenSpace);
	toRestart->SetRelativePosition(FVector2(0, -50));
	toRestart->SetRelativeScale(FVector2(1, 1));
	toRestart->SetActive(false);
	toRestart->SetLayer(Define::Disable);

	// ======================== toSelect
	toSelect->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toSelect->SetDrawType(EDrawType::ScreenSpace);
	toSelect->SetRelativePosition(FVector2(0, 50));
	toSelect->SetRelativeScale(FVector2(1, 1));
	toSelect->SetActive(false);
	toSelect->SetLayer(Define::Disable);

	// ======================== toOption
	toOption->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toOption->SetDrawType(EDrawType::ScreenSpace);
	toOption->SetRelativePosition(FVector2(0, 150));
	toOption->SetRelativeScale(FVector2(1, 1));
	toOption->SetActive(false);
	toOption->SetLayer(Define::Disable);
	
	// ======================== toMain
	toMain->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toMain->SetDrawType(EDrawType::ScreenSpace);
	toMain->SetRelativePosition(FVector2(0, 250));
	toMain->SetRelativeScale(FVector2(1, 1));
	toMain->SetActive(false);
	toMain->SetLayer(Define::Disable);

	// ======================== soundButton
	bgmPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	bgmPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
	{
		const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
		const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = -350.f;
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
		const float marginX = 560.f;
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
		const float marginX = -350.f;
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
		const float marginX = 560.f;
		const float marginY = 10.7f;
		const float rowY = panelH * 0.25f;
		sfxMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY - marginY));
	}
	sfxMinusButton->SetActive(false);
	sfxMinusButton->SetLayer(Define::Disable);

	// ========================= Delegate
	pauseButton->SetStateAction(Define::EButtonState::Hover, [pauseButton]()
		{
			pauseButton->StartHoverPulse(0.8f, 0.04f);
			pauseButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::HoverLeave, [pauseButton]()
		{
			pauseButton->StopHoverPulse();
			pauseButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::Release, [pauseButton]()
		{
			pauseButton->StopHoverPulse();
			pauseButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::Pressed, [this,
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		sound->PauseByType(SoundType::BGM);
		sound->PauseByType(SoundType::SFX);

		GamePlayManager::GetInstance().PauseGame();

		m_isPaused = !m_isPaused;

		pauseButton->SetActive(false);
		popUpTab->SetLayer(Define::PopupLayer);
		popUpTab->SetOpacity(1);

		pauseText->SetLayer(Define::PopupTextLayer);
		closeButton->SetActive(true);
		closeButton->SetLayer(Define::PopupButtonLayer);

		toMain->SetActive(true);
		toMain->SetLayer(Define::PopupButtonLayer);
		mainText->SetLayer(Define::PopupTextLayer);

		toRestart->SetActive(true);
		toRestart->SetLayer(Define::PopupButtonLayer);
		restartText->SetLayer(Define::PopupTextLayer);

		toOption->SetActive(true);
		toOption->SetLayer(Define::PopupButtonLayer);
		optionText->SetLayer(Define::PopupTextLayer);

		toSelect->SetActive(true);
		toSelect->SetLayer(Define::PopupButtonLayer);
		selectText->SetLayer(Define::PopupTextLayer);
		});

	//closeButton->SetStateAction(Define::EButtonState::Hover, [closeButton]()
	//	{
	//		closeButton->StartHoverPulse(0.8f, 0.04f);
	//		closeButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	//	});

	//closeButton->SetStateAction(Define::EButtonState::HoverLeave, [closeButton]()
	//	{
	//		closeButton->StopHoverPulse();
	//		closeButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	//	});

	//closeButton->SetStateAction(Define::EButtonState::Release, [closeButton]()
	//	{
	//		closeButton->StopHoverPulse();
	//		closeButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	//	});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [this,
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText, soundControl,
		bgmControl, sfxControl, bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton,
		optionTabText, optionTabBGMText, optionTabSFXText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		sound->ResumeByType(SoundType::BGM);
		sound->ResumeByType(SoundType::SFX);
		
		GamePlayManager::GetInstance().ResumeGame();

		m_isPaused = !m_isPaused;

		pauseText->SetLayer(Define::Disable);
		pauseButton->SetActive(true);
		popUpTab->SetLayer(Define::Disable);
		popUpTab->SetOpacity(0);

		closeButton->SetActive(false);
		closeButton->SetLayer(Define::Disable);

		toMain->SetActive(false);
		toMain->SetLayer(Define::Disable);
		mainText->SetLayer(Define::Disable);

		toRestart->SetActive(false);
		toRestart->SetLayer(Define::Disable);
		restartText->SetLayer(Define::Disable);

		toOption->SetActive(false);
		toOption->SetLayer(Define::Disable);
		optionText->SetLayer(Define::Disable);

		toSelect->SetActive(false);
		toSelect->SetLayer(Define::Disable);
		selectText->SetLayer(Define::Disable);

		optionTabText->SetLayer(Define::Disable);
		optionTabBGMText->SetLayer(Define::Disable);
		optionTabSFXText->SetLayer(Define::Disable);

		soundControl->SetLayer(Define::Disable);
		bgmControl->SetLayer(Define::Disable);
		sfxControl->SetLayer(Define::Disable);

		bgmPlusButton->SetLayer(Define::Disable);
		bgmPlusButton->SetActive(false);
		bgmMinusButton->SetLayer(Define::Disable);
		bgmMinusButton->SetActive(false);
		sfxPlusButton->SetLayer(Define::Disable);
		sfxPlusButton->SetActive(false);
		sfxMinusButton->SetLayer(Define::Disable);
		sfxMinusButton->SetActive(false);
		});

	// bgmPlusButton Hover 효과 (시안색 글로우)
	bgmPlusButton->SetStateAction(Define::EButtonState::Hover, [bgmPlusButton]()
		{
			bgmPlusButton->StartHoverPulse(0.8f, 0.04f);
			bgmPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	bgmPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmPlusButton]()
		{
			bgmPlusButton->StopHoverPulse();
			bgmPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	bgmPlusButton->SetStateAction(Define::EButtonState::Release, [bgmPlusButton]()
		{
			bgmPlusButton->StopHoverPulse();
			bgmPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// bgmMinusButton Hover 효과 (시안색 글로우)
	bgmMinusButton->SetStateAction(Define::EButtonState::Hover, [bgmMinusButton]()
		{
			bgmMinusButton->StartHoverPulse(0.8f, 0.04f);
			bgmMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmMinusButton]()
		{
			bgmMinusButton->StopHoverPulse();
			bgmMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::Release, [bgmMinusButton]()
		{
			bgmMinusButton->StopHoverPulse();
			bgmMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// sfxPlusButton Hover 효과 (시안색 글로우)
	sfxPlusButton->SetStateAction(Define::EButtonState::Hover, [sfxPlusButton]()
		{
			sfxPlusButton->StartHoverPulse(0.8f, 0.04f);
			sfxPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxPlusButton]()
		{
			sfxPlusButton->StopHoverPulse();
			sfxPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::Release, [sfxPlusButton]()
		{
			sfxPlusButton->StopHoverPulse();
			sfxPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// sfxMinusButton Hover 효과 (시안색 글로우)
	sfxMinusButton->SetStateAction(Define::EButtonState::Hover, [sfxMinusButton]()
		{
			sfxMinusButton->StartHoverPulse(0.8f, 0.04f);
			sfxMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxMinusButton]()
		{
			sfxMinusButton->StopHoverPulse();
			sfxMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Release, [sfxMinusButton]()
		{
			sfxMinusButton->StopHoverPulse();
			sfxMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});


	bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl] {
		
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol += 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);

		bgmControl->SetProgress(vol);
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl] {
		
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol -= 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);

		bgmControl->SetProgress(vol);
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, sfxControl] {

		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx += 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);

		sfxControl->SetProgress(sfx);
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, sfxControl] {

		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx -= 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);

		sfxControl->SetProgress(sfx);
		});

	// 효과 (주황색 글로우)
	toSelect->SetStateAction(Define::EButtonState::Hover, [toSelect]()
		{
			toSelect->StartHoverPulse(0.8f, 0.04f);
			toSelect->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::HoverLeave, [toSelect]()
		{
			toSelect->StopHoverPulse();
			toSelect->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::Release, [toSelect]()
		{
			toSelect->StopHoverPulse();
			toSelect->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"SelectScene");
		});

	// 효과 (주황색 글로우)
	toOption->SetStateAction(Define::EButtonState::Hover, [toOption]()
		{
			toOption->StartHoverPulse(0.8f, 0.04f);
			toOption->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::HoverLeave, [toOption]()
		{
			toOption->StopHoverPulse();
			toOption->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::Release, [toOption]()
		{
			toOption->StopHoverPulse();
			toOption->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::Pressed, [sound,
		soundControl, bgmControl, sfxControl,
		bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton,
		pauseButton, closeButton, toOption, toMain, toRestart, toSelect, smallClose,
		optionTabText, optionTabBGMText, optionTabSFXText
	] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

		pauseButton->SetActive(false);
		closeButton->SetActive(false);
		toOption->SetActive(false);
		toMain->SetActive(false);
		toRestart->SetActive(false);
		toSelect->SetActive(false);

			smallClose->SetActive(true);
	smallClose->SetLayer(PopupButtonLayer);

		optionTabText->SetLayer(Define::PopupTextLayer);
		optionTabBGMText->SetLayer(Define::PopupTextLayer);
		optionTabSFXText->SetLayer(Define::PopupTextLayer);

		soundControl->SetLayer(Define::PopupPopLayer);
		bgmControl->SetLayer(Define::PopupObjectLayer);
		sfxControl->SetLayer(Define::PopupObjectLayer);

		bgmPlusButton->SetLayer(Define::PopupButtonLayer);
		bgmPlusButton->SetActive(true);
		bgmMinusButton->SetLayer(Define::PopupButtonLayer);
		bgmMinusButton->SetActive(true);
		sfxPlusButton->SetLayer(Define::PopupButtonLayer);
		sfxPlusButton->SetActive(true);
		sfxMinusButton->SetLayer(Define::PopupButtonLayer);
		sfxMinusButton->SetActive(true);
		});

	smallClose->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, toOption, toMain, toRestart, toSelect,
		smallClose, soundControl, bgmControl, sfxControl, bgmPlusButton, bgmMinusButton,
		sfxPlusButton, sfxMinusButton, sound, optionTabText, optionTabBGMText, optionTabSFXText

	] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");
			
		pauseButton->SetActive(true);
		closeButton->SetActive(true);
		toOption->SetActive(true);
		toMain->SetActive(true);
		toRestart->SetActive(true);
		toSelect->SetActive(true);

		smallClose->SetActive(false);
		smallClose->SetLayer(Define::Disable);

		optionTabText->SetLayer(Define::Disable);
		optionTabBGMText->SetLayer(Define::Disable);
		optionTabSFXText->SetLayer(Define::Disable);

		soundControl->SetLayer(Define::Disable);
		bgmControl->SetLayer(Define::Disable);
		sfxControl->SetLayer(Define::Disable);

		bgmPlusButton->SetLayer(Define::Disable);
		bgmPlusButton->SetActive(false);
		bgmMinusButton->SetLayer(Define::Disable);
		bgmMinusButton->SetActive(false);
		sfxPlusButton->SetLayer(Define::Disable);
		sfxPlusButton->SetActive(false);
		sfxMinusButton->SetLayer(Define::Disable);
		sfxMinusButton->SetActive(false);

		});

	// 효과 (주황색 글로우)
	toRestart->SetStateAction(Define::EButtonState::Hover, [toRestart]()
		{
			toRestart->StartHoverPulse(0.8f, 0.04f);
			toRestart->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::HoverLeave, [toRestart]()
		{
			toRestart->StopHoverPulse();
			toRestart->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::Release, [toRestart]()
		{
			toRestart->StopHoverPulse();
			toRestart->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::Pressed, [weak = WeakFromThis<StageWidgetScript>(), sound,
		pauseText, pauseButton, popUpTab, closeButton, toMain, mainText,
		toRestart, restartText, toOption, optionText, toSelect, selectText
	] {
		sound->PlayByName(L"UISound");

		// TODO : 재시작 코드
		if (weak.expired())return;
		std::wstring sceneName = weak->GetWorld()->GetName();
		SceneManager::GetInstance().ChangeScene(sceneName);

		pauseText->SetLayer(Define::Disable);
		pauseButton->SetActive(true);
		popUpTab->SetLayer(Define::Disable);
		popUpTab->SetOpacity(0);

		closeButton->SetActive(false);
		closeButton->SetLayer(Define::Disable);

		toMain->SetActive(false);
		toMain->SetLayer(Define::Disable);
		mainText->SetLayer(Define::Disable);

		toRestart->SetActive(false);
		toRestart->SetLayer(Define::Disable);
		restartText->SetLayer(Define::Disable);

		toOption->SetActive(false);
		toOption->SetLayer(Define::Disable);
		optionText->SetLayer(Define::Disable);

		toSelect->SetActive(false);
		toSelect->SetLayer(Define::Disable);
		selectText->SetLayer(Define::Disable);
		});

	// 효과 (주황색 글로우)
	toMain->SetStateAction(Define::EButtonState::Hover, [toMain]()
		{
			toMain->StartHoverPulse(0.8f, 0.04f);
			toMain->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::HoverLeave, [toMain]()
		{
			toMain->StopHoverPulse();
			toMain->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::Release, [toMain]()
		{
			toMain->StopHoverPulse();
			toMain->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::Pressed, [] {

		SceneManager::ChangeScene(L"TitleScene");
		});

	m_passedTimeText = m_owner->AddComponent<TextRenderComponent>();
	m_passedTimeText->SetText(std::wstring());
	m_passedTimeText->SetFontSize(45.0f);
	m_passedTimeText->SetFontFromFile(L"Fonts\\digital.ttf");
	m_passedTimeText->SetFont(L"Digital-7 Mono", L"en-US");
	m_passedTimeText->SetTextAlignment(ETextFormat::TopLeft);
	m_passedTimeText->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f + 59, -SCREEN_HEIGHT / 2.0f + 80));
	m_passedTimeText->SetColor(FColor(0, 234, 255, 255));
	m_passedTimeText->SetLayer(Define::NormalTextLayer);

	//m_killEnemyText = owner->AddComponent<TextRenderComponent>();
	//m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));
	//m_killEnemyText->SetTextAlignment(ETextFormat::TopLeft);
	//m_killEnemyText->SetRelativePosition(FVector2(20, 80));
	//m_killEnemyText->SetFontSize(32.0f);
	//m_killEnemyText->SetColor(FColor::Gold);
	//m_killEnemyText->m_layer = Define::NormalTextLayer;

	auto input = m_owner->AddComponent<InputComponent>();
	input->SetAction(input->GetHandle(), [this,
		sound, pauseButton, popUpTab, pauseText, closeButton, toMain, mainText,
		toRestart, restartText, toOption, optionText, toSelect, selectText,
		optionTabText, optionTabBGMText, optionTabSFXText,
		soundControl, bgmControl, sfxControl,bgmPlusButton,bgmMinusButton,sfxPlusButton,sfxMinusButton,
		smallClose
	] {
		if (GamePlayManager::GetInstance().IsCutScenePlaying()) return;

		if (!m_isPaused && Input::IsKeyPressed(VK_ESCAPE))
		{
			m_isPaused = !m_isPaused;

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			GamePlayManager::GetInstance().PauseGame();

			sound->PauseByType(SoundType::BGM);
			sound->PauseByType(SoundType::SFX);

			pauseButton->SetActive(false);
					popUpTab->SetLayer(Define::PopupLayer);
		popUpTab->SetOpacity(1);

					pauseText->SetLayer(Define::PopupTextLayer);
		closeButton->SetActive(true);
		closeButton->SetLayer(Define::PopupButtonLayer);

				toMain->SetActive(true);
	toMain->SetLayer(Define::PopupButtonLayer);
			mainText->SetLayer(Define::PopupTextLayer);

			toRestart->SetActive(true);
			toRestart->SetLayer(Define::PopupButtonLayer);
			restartText->SetLayer(Define::PopupTextLayer);

			toOption->SetActive(true);
			toOption->SetLayer(Define::PopupButtonLayer);
			optionText->SetLayer(Define::PopupTextLayer);

			toSelect->SetActive(true);
					toSelect->SetLayer(Define::PopupButtonLayer);
		selectText->SetLayer(Define::PopupTextLayer);
		}
		else if (m_isPaused && Input::IsKeyPressed(VK_ESCAPE))
		{
			m_isPaused = !m_isPaused;

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			GamePlayManager::GetInstance().ResumeGame();

			sound->ResumeByType(SoundType::BGM);
			sound->ResumeByType(SoundType::SFX);

			pauseText->SetLayer(Define::Disable);
			pauseButton->SetActive(true);
			popUpTab->SetLayer(Define::Disable);
			popUpTab->SetOpacity(0);

			closeButton->SetActive(false);
			closeButton->SetLayer(Define::Disable);

			toMain->SetActive(false);
			toMain->SetLayer(Define::Disable);
			mainText->SetLayer(Define::Disable);

			toRestart->SetActive(false);
			toRestart->SetLayer(Define::Disable);
			restartText->SetLayer(Define::Disable);

			toOption->SetActive(false);
			toOption->SetLayer(Define::Disable);
			optionText->SetLayer(Define::Disable);

			toSelect->SetActive(false);
			toSelect->SetLayer(Define::Disable);
			selectText->SetLayer(Define::Disable);

			optionTabText->SetLayer(Define::Disable);
			optionTabBGMText->SetLayer(Define::Disable);
			optionTabSFXText->SetLayer(Define::Disable);

			soundControl->SetLayer(Define::Disable);
			bgmControl->SetLayer(Define::Disable);
			sfxControl->SetLayer(Define::Disable);

			bgmPlusButton->SetLayer(Define::Disable);
			bgmPlusButton->SetActive(false);
			bgmMinusButton->SetLayer(Define::Disable);
			bgmMinusButton->SetActive(false);
			sfxPlusButton->SetLayer(Define::Disable);
			sfxPlusButton->SetActive(false);
			sfxMinusButton->SetLayer(Define::Disable);
			sfxMinusButton->SetActive(false);

			smallClose->SetActive(false);
			smallClose->SetLayer(Define::Disable);
		}
		});
}

void StageWidgetScript::OnEnd()
{
}

void StageWidgetScript::OnDestroy()
{
}

void StageWidgetScript::SetProgress()
{
	float bProgress = (float)GamePlayManager::GetInstance().batteryCount / (float)maxBattery;
	bProgress = bProgress > 1.0f ? 1.0f : bProgress;	// Battery : 0~1
	m_batteryProgress->SetProgress(bProgress);
}
