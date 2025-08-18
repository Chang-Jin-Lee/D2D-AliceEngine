/*
* @brief : 화면 가장자리에서 중앙으로 서서히 어두워지는 비네트(암전) 연출 위젯 스크립트입니다.
* @details :
*   - 오브젝트에 부착하면 내부에서 `SpriteRenderer`를 생성하여 ScreenSpace 기준으로 화면 전체를 덮습니다.
*   - Direct2D 이펙트(주로 Vignette + Opacity)를 사용해 가장자리부터 중앙으로 검게 수렴하는 효과를 만듭니다.
*   - 진행도는 `Update()`에서 시간 `t = elapsed / duration`으로 계산하며, `SetDuration()`으로 총 연출 시간을 지정할 수 있습니다.
*   - 최종 불투명도 상한은 `SetMaxEdgeAlpha()`로 설정합니다. 1.0이면 완전 암전까지 진행합니다.
*   - 여러 인스턴스를 동시에 생성할 경우, 비네트가 누적되어 중앙이 더욱 작게 보이는 연출이 발생할 수 있습니다(의도적 활용 가능).
*   - 씬 예시: `DemoScene2`에서 SpaceBar로 생성하여 테스트합니다.
*/
#pragma once
#include <Component/ScriptComponent.h>
#include <wrl/client.h>
struct ID2D1Effect;
class SpriteRenderer;

class VignetteWidgetScript : public ScriptComponent
{
public:
    void Initialize() override;
    void Update(const float& deltaSeconds) override;
    void OnStart() override;

    // 설정
    void SetDuration(float seconds) { m_duration = (seconds <= 0.f ? 0.01f : seconds); }
    void SetMaxEdgeAlpha(float a01) { m_maxEdgeAlpha = (a01 < 0.f ? 0.f : (a01 > 1.f ? 1.f : a01)); }

private:
    bool  m_active = true;
    float m_elapsed = 0.f;
    float m_duration = 1.0f;    // 완전 암전까지 걸리는 시간
    float m_maxEdgeAlpha = 1.0f; // 가장자리 최대 불투명도(최종 암전 시 1.0)
    // 단일 오버레이 스프라이트 (전체 화면 페이드)
    SpriteRenderer* m_overlay = nullptr;
    // D2D 이펙트 체인 (Vignette -> Opacity)
    Microsoft::WRL::ComPtr<ID2D1Effect> m_vignetteEffect;
    Microsoft::WRL::ComPtr<ID2D1Effect> m_opacityEffect;
    Microsoft::WRL::ComPtr<ID2D1Effect> m_spotDiffuseEffect; // 스팟 조명 기반 어둡게
    bool m_fullyClosedApplied = false; // 최종 완전 암전 전환 여부
    FVector2 screenSize;
};
