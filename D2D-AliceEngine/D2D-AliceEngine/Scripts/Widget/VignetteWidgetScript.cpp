#include "VignetteWidgetScript.h"
#include <Component/SpriteRenderer.h>
#include <Object/gameObject.h>
#include <Manager/UpdateTaskManager.h>
#include <Define/Define.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/CoordHelper.h>
#include <System/ScriptSystem.h>

void VignetteWidgetScript::Initialize()
{
    __super::Initialize();
    //REGISTER_SCRIPT_METHOD(OnStart);
    //REGISTER_SCRIPT_TICK(Update);
}

void VignetteWidgetScript::OnStart()
{
    // 시작 즉시 업데이트 루프로 등록
    //REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostUpdateWork);

    // 단일 오버레이 스프라이트 생성 (CutSceneWidget과 동일한 방법)
    // 카메라에 붙여 화면을 정확히 덮도록 함
    m_overlay = owner->AddComponent<SpriteRenderer>();
    m_overlay->SetDrawType(Define::EDrawType::ScreenSpace);
    // 화이트 1x1 텍스처를 사용하고 D2D 이펙트로 비네트 + 투명도 조절
    m_overlay->LoadData(L"CutScene/Transparent.png");
    // 화면 전체 크기로 스케일 고정 (매 프레임 보정)
    const float w = m_overlay->GetBitmapSizeX();
    const float h = m_overlay->GetBitmapSizeY();
    if (w <= 0 || h <= 0) return;
    m_overlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));
    m_overlay->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.5, 0.5)));
    m_overlay->SetLayer(999999); // 화면 최상위로
    // D2D Vignette + Opacity 이펙트 체인 구성
    if (auto* ctx = D2DRenderManager::GetD2DDevice())
    {
        // Vignette
        ctx->CreateEffect(CLSID_D2D1Vignette, m_vignetteEffect.GetAddressOf());
        // Opacity (알파 조절)
        ctx->CreateEffect(CLSID_D2D1Opacity, m_opacityEffect.GetAddressOf());
        if (m_vignetteEffect && m_opacityEffect && m_overlay->m_bitmap)
        {
            m_vignetteEffect->SetInput(0, m_overlay->m_bitmap.get());
            // 가장자리 색상: 검정(알파 1)
            m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_COLOR, D2D1::Vector4F(0, 0, 0, 1));
            // 시작 시 약한 강도
            m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, 0.0f);
            m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, 2.0f);

            // Opacity 이펙트에 비네트 출력 연결
            m_opacityEffect->SetInputEffect(0, m_vignetteEffect.Get());
            m_opacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, 0.0f);

            // SpriteRenderer가 이펙트를 그리도록 연결
            m_overlay->m_effect = m_opacityEffect;
        }
    }
}

void VignetteWidgetScript::Update(const float& deltaSeconds)
{ 
    __super::Update(deltaSeconds);
    if (!m_active) return;

    m_elapsed += deltaSeconds;
    float t = m_elapsed / m_duration;
    if (t > 1.0f) t = 1.0f;

    if (!m_overlay) return;

    // 화면 리사이즈 대응: 매 프레임 화면 크기에 맞게 스케일 유지
    const float w = m_overlay->GetBitmapSizeX();
    const float h = m_overlay->GetBitmapSizeY();
    if (w > 0 && h > 0)
    {
        m_overlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));
    }

    // D2D 이펙트 파라미터 갱신
    if (m_vignetteEffect && m_opacityEffect)
    {
        // 시간이 지날수록 비네트 강도 증가 + 전체 알파도 증가
        float strength = 16.0f * t; 
        float opacity  = m_maxEdgeAlpha * t; 
        // t가 1에 가까우면 완전 암전 보정: 강도와 알파를 충분히 크게 고정
        if (t >= 0.999f)
        {
            strength = 32.0f;   // 매우 강한 비네트
            opacity  = 1.0f;    // 완전 불투명
            m_fullyClosedApplied = true;
        }
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, strength);
        m_opacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, opacity);
    }
    else
    {
        // 이펙트 생성 실패 시 폴백: 단순 알파 페이드
        m_overlay->SetOpacity(m_maxEdgeAlpha * t);
    }
}
