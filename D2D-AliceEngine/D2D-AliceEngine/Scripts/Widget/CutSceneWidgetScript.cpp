#include "CutSceneWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/ButtonComponent.h>
#include <Component/AudioComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Helpers/Logger.h>
#include <GameManager/GamePlayManager.h>
#include <Component/ProgressBarComponent.h>
#include <Component/InputComponent.h>

void CutSceneWidgetScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void CutSceneWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 키보드 입력으로도 조작 가능
 //   if (!m_isTransitioning && m_canAdvance && (Input::IsKeyDown(VK_SPACE)))
	//{
	//	NextImage();
	//}
    //else if (!m_isTransitioning && Input::IsKeyDown(VK_LEFT))
    //{
    //    PrevImage();
    //}
 //   else if (!m_isTransitioning && Input::IsKeyDown(VK_ESCAPE))
	//{
	//	SkipCutScene();
	//}

    m_progress->SetProgress(m_pressValue / 500.0f);

    FVector2 mousePos = Input::GetMousePosition();

    m_progress->SetRelativePosition(FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 450));
    m_progressSprite->SetRelativePosition(FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 450));
    m_guideText->SetRelativePosition(
        CoordHelper::RatioCoordToScreen(m_guideText->GetRelativeSize(), FVector2(-0.5, -0.5)) +
        FVector2(mousePos.x - SCREEN_WIDTH / 2.0f, mousePos.y - 400));

    // 진행 중인 전환 스텝 처리
    if (m_isTransitioning)
    {
        StepTransition(TimerManager::GetInstance().unscaledDeltaTime);
    }

    m_seconds = TimerManager::GetInstance().unscaledDeltaTime;
}

void CutSceneWidgetScript::Awake()
{
    m_isAlive = true;
    m_isSpaceReleased = false;
    GamePlayManager::GetInstance().SetCutScenePlaying(m_isAlive);
}

void CutSceneWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->transform()->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

    m_seconds = 0.0f;   // 델타타임 초기화

    auto input = m_owner->AddComponent<InputComponent>();

    m_progressSprite = m_owner->AddComponent<SpriteRenderer>();
    m_progress = m_owner->AddComponent<ProgressBarComponent>();

    // ========= Progress
    m_progressSprite->LoadData(L"UI\\PressValue.png");
    m_progressSprite->SetLayer(Define::PopupObjectLayer + Define::CutSceneLayer);
    m_progressSprite->SetDrawType(EDrawType::ScreenSpace);
    m_progressSprite->SetRelativePosition(FVector2(0, 0));
    
    m_progress->LoadData(L"UI\\ProgressBar.png");
    m_progress->SetDrawType(EDrawType::ScreenSpace);
    m_progress->SetRelativePosition(FVector2(0, 0));
    m_progress->SetLayer(Define::PopupObjectLayer +1 + Define::CutSceneLayer);
    m_progress->SetProgress(m_pressValue);

	// 컷씬 이미지 로드
	LoadCutSceneImages();
	m_currentImageIndex = 0;

	// 컴포넌트들 생성
    // 월드 가림용 백드롭을 가장 먼저 생성하여 항상 뒤를 완전 차단
    EnsureBackdropOverlay();

    auto spacebar = m_owner->AddComponent<InputComponent>();

    m_background = m_owner->AddComponent<SpriteRenderer>();
    //m_nextButton = m_owner->AddComponent<ButtonComponent>();
    //m_prevButton = m_owner->AddComponent<ButtonComponent>();
	m_skipButton = m_owner->AddComponent<ButtonComponent>();

    //m_nextText = m_owner->AddComponent<TextRenderComponent>();
    //m_prevText = m_owner->AddComponent<TextRenderComponent>();
	m_skipText = m_owner->AddComponent<TextRenderComponent>();

	m_uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	m_uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);

    m_background->SetDrawType(Define::EDrawType::ScreenSpace);
    m_background->SetRelativePosition(FVector2(0, 0));
    // 배경 사이즈를 원하는 값으로 지정 (예: 1920x1080)
    // SpriteRenderer 엔진 전역 변경 없이, 여기서 스케일을 직접 계산해 적용
    // 원본 비트맵 크기 대비 목표 픽셀 크기로 스케일을 정합니다.
    const float targetW = 1920.f;
    const float targetH = 1080.f;
    // 아직 비트맵이 로드되지 않았으므로, 첫 ShowImage에서 실제 스케일을 적용합니다.
    // 임시로 layer만 설정 (상대 레이어 적용)
    m_background->SetLayer(0 + Define::CutSceneLayer);

	// Next 버튼 설정
    // Next 버튼 주석 처리

	// Prev 버튼 설정
    // Prev 버튼 주석 처리

	// Skip 버튼 설정
	m_skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	m_skipButton->SetRelativePosition(FVector2(0, 350));
	m_skipButton->SetRelativeScale(FVector2(1.f, 1.f));
    m_skipButton->SetLayer(Define::ButtonLayer + Define::CutSceneLayer);

	// 텍스트 설정
    // Next 텍스트 주석 처리

    // Prev 텍스트 주석 처리

	m_skipText->SetFontSize(45.0f);
	m_skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_skipText->SetText(L"다음으로");
	m_skipText->SetColor(FColor::White);
	FVector2 skipTextSize = m_skipText->GetRelativeSize();
	m_skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextSize, FVector2(-0.5, -0.5)));
    m_skipText->SetLayer(Define::ButtonTextLayer + Define::CutSceneLayer);
	m_skipText->RemoveFromParent();
	m_skipButton->AddChildComponent(m_skipText);

    // 안내 문구 생성
    m_guideText = m_owner->AddComponent<TextRenderComponent>();
    m_guideText->SetFontSize(25.0f);
    m_guideText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
    m_guideText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
    m_guideText->SetText(L"[SPACE]를 길게 눌러서 컷씬 건너뛰기");
    m_guideText->SetColor(FColor::White);
    m_guideText->SetLayer(Define::PopupTextLayer + Define::CutSceneLayer);
    //m_guideText->RemoveFromParent();
    //m_skipButton->AddChildComponent(m_guideText);
    // 시작 시 감춤
    //m_guideAlpha = 0.0f;
    //m_guideText->SetOpacity(0.0f);
    //m_guideFadeElapsed = 0.0f;

	// 버튼 이벤트 설정
    // Next 버튼 핸들러 주석 처리

    // Prev 버튼 핸들러 주석 처리

    input->SetAction(input->GetHandle(), [this] {
        SkipInput(m_seconds);

        });

	m_skipButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
		m_uiSound->StopByName(L"UISound");
		m_uiSound->PlayByName1(L"UISound", 0.45f);

        NextImage();
        //GamePlayManager::GetInstance().StartGame();
        
		//SkipCutScene();
	});

	// 호버 효과 추가
	// Next Button 효과 (파란색 글로우)
	//m_nextButton->SetStateAction(Define::EButtonState::Hover, [this]()
	//{
	//	m_nextButton->StartHoverPulse(0.8f, 0.04f);
	//	m_nextButton->StartEffectAnimation(0.3f, 1.2f, FColor::Blue);
	//});
    //
	//m_nextButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	//{
	//	m_nextButton->StopHoverPulse();
	//	m_nextButton->StartEffectAnimation(0.2f, 0.0f, FColor::Blue);
	//});
    //
	//m_nextButton->SetStateAction(Define::EButtonState::Release, [this]()
	//{
	//	m_nextButton->StopHoverPulse();
	//	m_nextButton->StartEffectAnimation(0.1f, 0.0f, FColor::Blue);
	//});
    //
	//// Prev Button 효과 (주황색 글로우)
	//m_prevButton->SetStateAction(Define::EButtonState::Hover, [this]()
	//{
	//	m_prevButton->StartHoverPulse(0.8f, 0.04f);
	//	m_prevButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	//});
    //
	//m_prevButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	//{
	//	m_prevButton->StopHoverPulse();
	//	m_prevButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	//});
    //
	//m_prevButton->SetStateAction(Define::EButtonState::Release, [this]()
	//{
	//	m_prevButton->StopHoverPulse();
	//	m_prevButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	//});

	// Skip Button 효과 (빨간색 글로우)
	m_skipButton->SetStateAction(Define::EButtonState::Hover, [this]()
	{
		m_skipButton->StartHoverPulse(0.8f, 0.04f);
		m_skipButton->StartEffectAnimation(0.3f, 1.2f, FColor::Red);
	});

	m_skipButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	{
		m_skipButton->StopHoverPulse();
		m_skipButton->StartEffectAnimation(0.2f, 0.0f, FColor::Red);
	});

	m_skipButton->SetStateAction(Define::EButtonState::Release, [this]()
	{
		m_skipButton->StopHoverPulse();
		m_skipButton->StartEffectAnimation(0.1f, 0.0f, FColor::Red);
	});

	// 이미지 이어서 표시
    int startIndex = 0;

    m_currSceneName = SceneManager::GetInstance().m_currentScene->GetName();

    if (m_currSceneName == Define::Scene_Stage1)
    {
        startIndex = 0;
    }

    if (m_currSceneName == Define::Scene_Stage2)
    {
        startIndex = 2;
    }

    if (m_currSceneName == Define::Scene_Stage3)
    {
        startIndex = 3;
    }

	ShowImage(startIndex);

    // 각 컷마다 2초 지연 후 가이드 표시
    ShowGuideAfterDelay();
}

void CutSceneWidgetScript::OnEnd()
{
    m_isAlive = false;
    StopGuideTimers();

    GamePlayManager::GetInstance().SetCutScenePlaying(m_isAlive);
}

void CutSceneWidgetScript::OnDestroy()
{
    m_isAlive = false;
    StopGuideTimers();
    m_guideText = nullptr;

    GamePlayManager::GetInstance().SetCutScenePlaying(m_isAlive);
}

void CutSceneWidgetScript::SkipInput(float deltaSeconds)
{
    if (m_currSceneName == Define::Scene_Stage1 && Input::IsKeyReleased(VK_SPACE))
    {
        m_isSpaceReleased = true;
    }
    else if (m_currSceneName == Define::Scene_Stage2 || m_currSceneName == Define::Scene_Stage3)
    {
        m_isSpaceReleased = true;
    }

    if (Input::IsKeyDown(VK_SPACE))
    {
        if (!m_isSpaceReleased) return;

        m_pressValue += 4.5f * deltaSeconds * 300;

        if (m_pressValue >= 500.0f)
        {
            SkipCutScene();
            GamePlayManager::GetInstance().StartGame();
        }
    }

    m_pressValue -= 1.5f * deltaSeconds * 300;
    if (m_pressValue <= 0.0f) m_pressValue = 0.0f;
}

void CutSceneWidgetScript::LoadCutSceneImages()
{
	// 컷씬 이미지 경로들을 추가
	m_cutSceneImages.clear();
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\stage_1_scene01.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage2\\stage_2_scene02.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage2\\stage_2_scene03.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage3\\stage_3_scene04.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage3\\stage_3_scene05.png");
}

void CutSceneWidgetScript::ShowImage(int index)
{
	if (index < 0 || index >= m_cutSceneImages.size())
		return;

	m_currentImageIndex = index;

    // 씬별 최대 허용 인덱스 지정
    std::unordered_map<std::wstring, int> maxIndexPerScene = {
        { Define::Scene_Stage1, 1 }, // 인덱스 1까지 허용
        { Define::Scene_Stage2, 2 }, // 인덱스 2까지 허용
        { Define::Scene_Stage3, 4 }, // 인덱스 4까지 허용
    };

    std::wstring currScene = SceneManager::GetInstance().m_currentScene->GetName();

    auto it = maxIndexPerScene.find(currScene);
    if (it != maxIndexPerScene.end())
    {
        int maxAllowedIndex = it->second;
        if (index >= maxAllowedIndex)
        {
            m_skipText->SetText(L"건너뛰기");

            m_skipButton->SetStateAction(Define::EButtonState::Pressed, [this]()
                {
                    m_uiSound->StopByName(L"UISound");
                    m_uiSound->PlayByName1(L"UISound", 0.45f);

                    SkipCutScene();
                    GamePlayManager::GetInstance().StartGame();
                });
        }

        // 만약 index가 maxAllowedIndex를 넘어가면 로드하지 않고 리턴
        if (index > maxAllowedIndex)
            return;
    }

    // 배경 이미지 변경하고 원하는 픽셀 사이즈(1920x1080)로 스케일 적용
    m_background->LoadData(m_cutSceneImages[index]);
    {
        const float targetW = 1920.f;
        const float targetH = 1080.f;
        const float bmpW = m_background->GetBitmapSizeX();
        const float bmpH = m_background->GetBitmapSizeY();
        if (bmpW > 0.f && bmpH > 0.f)
        {
            const FVector2 scale(targetW / bmpW, targetH / bmpH);
            m_background->SetRelativeScale(scale);
        }
    }

    // 이전 오버레이 제거
    ClearOverlays();
    // 해당 페이지에 등록된 추가 스프라이트 생성
    auto overlayIt = m_pageOverlays.find(index);
    if (overlayIt != m_pageOverlays.end())
    {
        for (const auto& desc : overlayIt->second)
        {
            auto* spr = m_owner->AddComponent<SpriteRenderer>();
            spr->SetDrawType(Define::EDrawType::ScreenSpace);
            spr->LoadData(desc.path);
            // 사이즈 지정이 있으면 스케일로 적용
            const float w = spr->GetBitmapSizeX();
            const float h = spr->GetBitmapSizeY();
            if (w > 0.f && h > 0.f && desc.size.x > 0.f && desc.size.y > 0.f)
            {
                spr->SetRelativeScale(FVector2(desc.size.x / w, desc.size.y / h));
            }
            spr->SetRelativePosition(desc.position);
            spr->SetLayer(desc.layer + Define::CutSceneLayer);
            m_activeOverlays.push_back(spr);
        }
    }

    // 버튼 활성화/텍스트 업데이트 (Next/Prev는 현재 주석 처리)
    //{
    //    const bool hasPrev = index > 0;
    //    const bool isLast = index >= static_cast<int>(m_cutSceneImages.size()) - 1;

    //    // 진행 가능 여부 초기화 및 가이드 리셋
    //    m_canAdvance = false;
    //    StopGuideTimers();
    //    if (m_guideText)
    //    {
    //        m_guideAlpha = 0.0f;
    //        m_guideText->SetOpacity(0.0f);
    //    }
    //    // 2초 뒤 가이드 출력
    //    ShowGuideAfterDelay();
    //}

	OutputDebugStringW((L"Showing cutscene image: " + std::to_wstring(index + 1) + L"/" + std::to_wstring(m_cutSceneImages.size()) + L"\n").c_str());
}

void CutSceneWidgetScript::NextImage()
{
    if (m_isTransitioning) return;
    if (m_currentImageIndex < m_cutSceneImages.size() - 1)
    {
        StartTransitionTo(m_currentImageIndex + 1, 0.8f);
    }
	else
	{
        // 마지막 이미지에서 다음 버튼을 누르면 컷씬 종료
        GamePlayManager::GetInstance().StartGame();
        TimerManager::GetInstance().SetGlobalTimeScale(1.0f);
        // 리소스 정리 및 컴포넌트 파괴
        m_isTransitioning = false;
        ClearOpacityOnAll();
        ClearOverlays();
        if (m_fadeOverlay)
        {
            m_fadeOverlay->RemoveFromParent();
            m_fadeOverlay->Release();
            m_fadeOverlay = nullptr;
        }
        if (m_backdropOverlay)
        {
            m_backdropOverlay->RemoveFromParent();
            m_backdropOverlay->Release();
            m_backdropOverlay = nullptr;
        }
        if (m_background)
        {
            m_background->RemoveFromParent();
            m_background->Release();
            m_background = nullptr;
        }
        GetWorld()->RemoveObject(owner.Get());
	}
}

void CutSceneWidgetScript::PrevImage()
{
    if (m_isTransitioning) return;
    if (m_currentImageIndex > 0)
    {
        StartTransitionTo(m_currentImageIndex - 1, 0.8f);
    }
}

void CutSceneWidgetScript::SkipCutScene()
{
	// 컷씬 종료 - 다음 씬으로 이동 (예: 게임 씬 또는 타이틀 씬)
	OutputDebugStringW(L"CutScene skipped or finished!\n");
    // 외부에서 지정한 다음 씬으로 이동
    //SceneManager::ChangeScene(m_nextSceneName.c_str());
    // 글로벌 타임스케일 복구
    TimerManager::GetInstance().SetGlobalTimeScale(1.0f);

    // 리소스 정리 및 컴포넌트 파괴
    m_isTransitioning = false;
    ClearOpacityOnAll();
    ClearOverlays();
    if (m_fadeOverlay)
    {
        m_fadeOverlay->RemoveFromParent();
        m_fadeOverlay->Release();
        m_fadeOverlay = nullptr;
    }
    if (m_backdropOverlay)
    {
        m_backdropOverlay->RemoveFromParent();
        m_backdropOverlay->Release();
        m_backdropOverlay = nullptr;
    }
    if (m_background)
    {
        m_background->RemoveFromParent();
        m_background->Release();
        m_background = nullptr;
    }
    // 자신 제거
    GetWorld()->RemoveObject(owner.Get());
}

void CutSceneWidgetScript::ShowGuideAfterDelay()
{
    TimerManager::GetInstance().ClearTimer(m_guideDelayTimer);
    TimerManager::GetInstance().SetTimer(
        m_guideDelayTimer,
        [this]() {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideDelayTimer); return; }
            // 2초 경과 후 가이드 페이드 인 시작 및 입력 허용
            m_canAdvance = true;
            //StartGuideFadeIn(0.3f);
        },
        2.0f,
        false,
        2.0f
    );
}

void CutSceneWidgetScript::StartGuideFadeIn(float durationSec)
{
    TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
    m_guideAlpha = 0.0f;
    m_guideFadeElapsed = 0.0f;
    TimerManager::GetInstance().SetTimer(
        m_guideFadeTimer,
        [this, durationSec]() mutable {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideFadeTimer); return; }
            const float dt = 1.0f / 60.0f;
            m_guideFadeElapsed += dt;
            float t = m_guideFadeElapsed / durationSec;
            if (t > 1.0f) t = 1.0f;
            m_guideAlpha = t;
            SetGuideAlpha(m_guideAlpha);
            if (t >= 1.0f)
            {
                TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
                // 페이드 인 완료 후 반짝임 시작
            }
        },
        0.0f,
        true,
        0.0f
    );
}

void CutSceneWidgetScript::StartGuideBlink(float intervalSec)
{
    TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer);
    // 0.5초마다 토글
    TimerManager::GetInstance().SetTimer(
        m_guideBlinkTimer,
        [this, visible = true]() mutable {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer); return; }
            visible = !visible;
            SetGuideAlpha(visible ? 1.0f : 0.3f);
        },
        intervalSec,
        true,
        intervalSec
    );
}

void CutSceneWidgetScript::StopGuideTimers()
{
    TimerManager::GetInstance().ClearTimer(m_guideDelayTimer);
    TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
    TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer);
}

void CutSceneWidgetScript::SetGuideAlpha(float a01)
{
    if (!m_guideText) return;
    if (a01 < 0.f) a01 = 0.f; if (a01 > 1.f) a01 = 1.f;
    m_guideText->SetOpacity(a01);
}

// 모든 관련 스프라이트에 동일 Opacity 적용 (배경 + 오버레이)
void CutSceneWidgetScript::ApplyOpacityToAll(float alpha)
{
    if (m_background) m_background->SetOpacity(alpha);
    for (auto* spr : m_activeOverlays)
        if (spr) spr->SetOpacity(alpha);
}

void CutSceneWidgetScript::ClearOpacityOnAll()
{
    if (m_background) m_background->ClearEffect();
    for (auto* spr : m_activeOverlays)
        if (spr) spr->ClearEffect();
}

// 페이드 아웃하고 나서 페이지 갱신 그 이후 페이드 인
void CutSceneWidgetScript::StartTransitionTo(int targetIndex, float durationSec)
{
    if (targetIndex < 0 || targetIndex >= (int)m_cutSceneImages.size()) return;
    m_isTransitioning = true;
    m_transitionHalf = durationSec * 0.5f;
    m_transitionElapsed = 0.f;
    m_transitionSwitchover = false;
    m_targetIndex = targetIndex;

    // 초기 상태
    ApplyOpacityToAll(1.f);
    EnsureFadeOverlay();
            if (m_fadeOverlay) { m_fadeOverlay->SetOpacity(0.f); m_fadeOverlay->SetLayer(9999 + Define::CutSceneLayer); }
}

void CutSceneWidgetScript::StepTransition(float dt)
{
    // 컷씬 진행 중에는 타임스케일 0이더라도 자체 dt로 동작해야 하므로 전달된 dt만 사용
    m_transitionElapsed += dt;
    float t = Math::Clamp(m_transitionElapsed / m_transitionHalf, 0.f, 1.f);
    float eased = t * t * (3.f - 2.f * t);

    if (!m_transitionSwitchover)
    {
        // 1 - 0 페이드 아웃
        ApplyOpacityToAll(1.f - eased);
        if (m_fadeOverlay) m_fadeOverlay->SetOpacity(eased * 0.4f);
        if (t >= 1.f)
        {
            ShowImage(m_targetIndex);
            m_transitionElapsed = 0.f;
            m_transitionSwitchover = true;
        }
    }
    else
    {
        // 0 - 1 페이드 인
        ApplyOpacityToAll(eased);
        if (m_fadeOverlay) m_fadeOverlay->SetOpacity((1.f - eased) * 0.4f);
        if (t >= 1.f)
        {
            ClearOpacityOnAll();
            if (m_fadeOverlay) { m_fadeOverlay->SetOpacity(0.f); m_fadeOverlay->SetLayer(Define::Disable); }
            m_isTransitioning = false;
            m_transitionElapsed = 0.f;
            m_transitionSwitchover = false;
            m_targetIndex = -1;
        }
    }
}

// 오버레이 관리 유틸
void CutSceneWidgetScript::ClearOverlays()
{
    for (auto* spr : m_activeOverlays)
    {
        if (spr)
        {
            spr->RemoveFromParent();
            spr->Release();
        }
    }
    m_activeOverlays.clear();
}

void CutSceneWidgetScript::AddSpriteToPage(int pageIndex, const CutSpriteDesc& desc)
{
    m_pageOverlays[pageIndex].push_back(desc);
}

void CutSceneWidgetScript::EnsureFadeOverlay()
{
    if (m_fadeOverlay) return;
    // 전체 화면을 덮는 검은 1x1 텍스처 필요: CutScene\\Black.png
    m_fadeOverlay = m_owner->AddComponent<SpriteRenderer>();
    m_fadeOverlay->SetDrawType(Define::EDrawType::ScreenSpace);
    m_fadeOverlay->LoadData(L"CutScene\\Black.png");
    const float w = m_fadeOverlay->GetBitmapSizeX();
    const float h = m_fadeOverlay->GetBitmapSizeY();
    if (w > 0 && h > 0)
    {
        m_fadeOverlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));
    }
    m_fadeOverlay->SetRelativePosition(FVector2(0, 0));
    // 페이드 오버레이는 컷씬 요소들보다 위 (상대 레이어 적용)
    m_fadeOverlay->SetLayer(9999 + Define::CutSceneLayer);
}

void CutSceneWidgetScript::EnsureBackdropOverlay()
{
    if (m_backdropOverlay) return;
    // 전체 화면을 덮는 검은 1x1 텍스처를 가장 뒤 레이어에 깔아 월드를 완전히 차단
    m_backdropOverlay = m_owner->AddComponent<SpriteRenderer>();
    m_backdropOverlay->SetDrawType(Define::EDrawType::ScreenSpace);
    m_backdropOverlay->LoadData(L"CutScene\\White.png");
    const float bw = m_backdropOverlay->GetBitmapSizeX();
    const float bh = m_backdropOverlay->GetBitmapSizeY();
    if (bw > 0 && bh > 0)
    {
        m_backdropOverlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / bw, Define::SCREEN_HEIGHT / bh));
    }
    m_backdropOverlay->SetRelativePosition(FVector2(0, 0));
    // 컷씬 전체보다 뒤에 위치 (상대 레이어 적용)
    m_backdropOverlay->SetLayer(-10000 + Define::CutSceneLayer);
}
