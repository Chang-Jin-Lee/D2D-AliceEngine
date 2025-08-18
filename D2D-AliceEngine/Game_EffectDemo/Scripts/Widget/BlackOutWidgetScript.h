/*
* @brief : 라이팅 기반의 블랙아웃(암전) 위젯 스크립트입니다.
* @details :
*   - 오브젝트에 부착하면 내부에서 `SpriteRenderer`를 생성하여 ScreenSpace 기준으로 화면 전체를 덮습니다.
*   - Direct2D 이펙트(Vignette + Spot/Point Lighting + Opacity + 선택 CrossFade)를 조합해
*     가장자리부터 중앙으로 어두워지며 최종적으로 완전 검정으로 전환되는 연출을 제공합니다.
*   - `SetDuration(seconds)`로 총 연출 시간을, `SetMaxEdgeAlpha(a01)`로 최종 불투명도 상한을 제어합니다.
*   - `SetMode()`로 라이팅 타입(SpotDiffuse/PointDiffuse/SpotSpecular/VignetteOnly)을 선택할 수 있습니다.
*/
#pragma once
#include <Component/ScriptComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;
struct ID2D1Bitmap1;
class SpriteRenderer;

enum class EBlackOutLightingMode
{
    VignetteOnly,
    PointDiffuse,
    SpotDiffuse,
    SpotSpecular,
};

class BlackOutWidgetScript : public ScriptComponent
{
public:
    void Initialize() override;
    void OnStart() override;
    void Update(const float& deltaSeconds) override;

    void SetDuration(float seconds) { m_duration = (seconds <= 0.f ? 0.01f : seconds); }
    void SetMaxEdgeAlpha(float a01) { m_maxEdgeAlpha = (a01 < 0.f ? 0.f : (a01 > 1.f ? 1.f : a01)); }
    void SetMode(EBlackOutLightingMode mode) { m_mode = mode; }
    void UseCrossFade(bool use) { m_useCrossFade = use; }

private:
    // 진행 상태
    bool  m_active = true;
    float m_elapsed = 0.f;
    float m_duration = 1.6f;
    float m_maxEdgeAlpha = 1.0f;

    // 렌더 오버레이
    SpriteRenderer* m_overlay = nullptr;
    int m_overlayLayer = 999999;

    // 이펙트들
    Microsoft::WRL::ComPtr<ID2D1Effect> m_vignetteEffect;
    Microsoft::WRL::ComPtr<ID2D1Effect> m_lightEffect;   // Spot/Point/Specular
    Microsoft::WRL::ComPtr<ID2D1Effect> m_opacityEffect; // 최종 알파
    Microsoft::WRL::ComPtr<ID2D1Effect> m_crossFadeEffect; // 선택: 블랙으로 완전 전환
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_blackBitmap;     // CrossFade용

    // 옵션
    EBlackOutLightingMode m_mode = EBlackOutLightingMode::SpotDiffuse;
    bool m_useCrossFade = true;
};


