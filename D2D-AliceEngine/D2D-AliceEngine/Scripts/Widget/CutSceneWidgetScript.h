#pragma once
#include <Component/ScriptComponent.h>
#include <vector>
#include <unordered_map>
#include <Manager/TimerManager.h>

class gameObject;
class SpriteRenderer;
class ButtonComponent;
class TextRenderComponent;
class AudioComponent;
class ProgressBarComponent;

class CutSceneWidgetScript : public ScriptComponent
{
public:
    // 컷씬 종료 시 전환할 씬 이름
    std::wstring m_nextSceneName = L"KangeTest";
    std::wstring m_currSceneName;
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

    // 한 컷에 여러 스프라이트를 배치하고 싶을 때 사용
    struct CutSpriteDesc {
        std::wstring path;
        FVector2 position{0,0};
        FVector2 size{0,0}; // 픽셀 단위; 0이면 원본 비율 유지(미사용)
        int layer{100};
    };
    void AddSpriteToPage(int pageIndex, const CutSpriteDesc& desc);

private:
	gameObject* m_owner;
	SpriteRenderer* m_background;
	ButtonComponent* m_nextButton;
	ButtonComponent* m_prevButton;
	ButtonComponent* m_skipButton;
	TextRenderComponent* m_nextText;
	TextRenderComponent* m_prevText;
	TextRenderComponent* m_skipText;
    TextRenderComponent* m_guideText{ nullptr };
	AudioComponent* m_uiSound;

    SpriteRenderer* m_progressSprite;
    ProgressBarComponent* m_progress;
    float m_pressValue;

    void SkipInput(float deltaSeconds);
    float m_seconds = 0.0f;
    bool m_isSpaceReleased = false;

	// 컷씬 관련
	std::vector<std::wstring> m_cutSceneImages;
	int m_currentImageIndex;

    // 페이지별 추가 스프라이트(오버레이)
    std::unordered_map<int, std::vector<CutSpriteDesc>> m_pageOverlays;
    std::vector<SpriteRenderer*> m_activeOverlays;

    // 페이드용 오버레이
    SpriteRenderer* m_fadeOverlay{ nullptr };
    // 월드 가림용 백드롭(항상 컷씬 뒤에서 월드를 완전 차단)
    SpriteRenderer* m_backdropOverlay{ nullptr };

	void LoadCutSceneImages();
	void ShowImage(int index);
	void NextImage();
	void PrevImage();
	void SkipCutScene();
    void ShowGuideAfterDelay();
    void StartGuideFadeIn(float durationSec = 0.3f);
    void StartGuideBlink(float intervalSec = 0.5f);
    void StopGuideTimers();
    void SetGuideAlpha(float a01);

    void ClearOverlays();

    // Fade transition helpers
    void StartTransitionTo(int targetIndex, float durationSec = 0.6f);
    void StepTransition(float dt);
    void ApplyOpacityToAll(float alpha);
    void ClearOpacityOnAll();
    void EnsureFadeOverlay();
    void EnsureBackdropOverlay();

    bool m_isTransitioning{ false };
    float m_transitionElapsed{ 0.f };
    bool m_transitionSwitchover{ false };
    float m_transitionHalf{ 0.3f };
    int m_targetIndex{ -1 };
    int m_relativeLayer{ 50000 };

    // Guide control
    bool m_canAdvance{ false };
    float m_guideAlpha{ 0.f };
    float m_guideFadeElapsed{ 0.f };
    bool m_isAlive{ false };
    FTimerHandle m_guideDelayTimer;
    FTimerHandle m_guideFadeTimer;
    FTimerHandle m_guideBlinkTimer;
};
