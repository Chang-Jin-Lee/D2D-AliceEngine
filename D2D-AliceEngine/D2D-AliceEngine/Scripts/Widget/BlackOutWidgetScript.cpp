#include "BlackOutWidgetScript.h"
#include <Component/SpriteRenderer.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/CoordHelper.h>
#include <System/ScriptSystem.h>
#include <Define/Define.h>
#include <Manager/UpdateTaskManager.h>

void BlackOutWidgetScript::Initialize()
{
    __super::Initialize();
    //REGISTER_SCRIPT_METHOD(OnStart);
    //REGISTER_SCRIPT_TICK(Update);
}

void BlackOutWidgetScript::OnStart()
{
    //REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostUpdateWork);

    // 오버레이 생성
    m_overlay = owner->AddComponent<SpriteRenderer>();
    m_overlay->SetDrawType(Define::EDrawType::ScreenSpace);
    m_overlay->LoadData(L"CutScene/Transparent.png");
    const float w = m_overlay->GetBitmapSizeX();
    const float h = m_overlay->GetBitmapSizeY();
    if (w <= 0 || h <= 0) return;
    m_overlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));
    m_overlay->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.5, 0.5)));
    m_overlay->SetLayer(m_overlayLayer);

    // 이펙트 체인 구성
    if (auto* ctx = D2DRenderManager::GetD2DDevice())
    {
        // Vignette
        ctx->CreateEffect(CLSID_D2D1Vignette, m_vignetteEffect.GetAddressOf());
        if (!m_vignetteEffect) return;
        m_vignetteEffect->SetInput(0, m_overlay->m_bitmap.get());
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_COLOR, D2D1::Vector4F(0, 0, 0, 1));
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, 0.0f);
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, 2.0f);

        // Lighting
        switch (m_mode)
        {
        case EBlackOutLightingMode::PointDiffuse:
            ctx->CreateEffect(CLSID_D2D1PointDiffuse, m_lightEffect.GetAddressOf());
            break;
        case EBlackOutLightingMode::SpotDiffuse:
            ctx->CreateEffect(CLSID_D2D1SpotDiffuse, m_lightEffect.GetAddressOf());
            break;
        case EBlackOutLightingMode::SpotSpecular:
            ctx->CreateEffect(CLSID_D2D1SpotSpecular, m_lightEffect.GetAddressOf());
            break;
        case EBlackOutLightingMode::VignetteOnly:
        default:
            break;
        }

        // Opacity
        ctx->CreateEffect(CLSID_D2D1Opacity, m_opacityEffect.GetAddressOf());
        if (!m_opacityEffect) return;

        // CrossFade (선택)
        if (m_useCrossFade)
        {
            ctx->CreateEffect(CLSID_D2D1CrossFade, m_crossFadeEffect.GetAddressOf());
            D2DRenderManager::GetInstance().CreateBitmapFromFile((Define::BASE_RESOURCE_PATH + L"CutScene/Black.png").c_str(), m_blackBitmap.GetAddressOf());
        }

        // 연결: bitmap -> vignette -> (lighting?) -> opacity -> (crossfade?) -> SpriteRenderer
        ID2D1Effect* last = m_vignetteEffect.Get();
        if (m_lightEffect)
        {
            m_lightEffect->SetInputEffect(0, last);
            last = m_lightEffect.Get();
        }
        m_opacityEffect->SetInputEffect(0, last);
        last = m_opacityEffect.Get();

        if (m_crossFadeEffect && m_blackBitmap)
        {
            m_crossFadeEffect->SetInputEffect(0, last);
            m_crossFadeEffect->SetInput(1, m_blackBitmap.Get());
            m_crossFadeEffect->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, 0.0f);
            last = m_crossFadeEffect.Get();
        }

        m_overlay->m_effect = last;
    }
}

void BlackOutWidgetScript::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    if (!m_active || !m_overlay) return;

    m_elapsed += deltaSeconds;
    float t = m_elapsed / m_duration; if (t > 1.0f) t = 1.0f;

    // 화면 크기 유지
    const float w = m_overlay->GetBitmapSizeX();
    const float h = m_overlay->GetBitmapSizeY();
    if (w > 0 && h > 0)
        m_overlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));

    // Vignette 진행
    if (m_vignetteEffect)
    {
        float strength = 16.0f * t; if (t >= 0.999f) strength = 32.0f;
        float transition = 2.0f - 1.5f * t; if (transition < 0.2f) transition = 0.2f;
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, strength);
        m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, transition);
    }

    // Lighting 진행
    if (m_lightEffect)
    {
        const FVector2 center = CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f));
        const D2D1_VECTOR_3F lightPos = { center.x, center.y, 50.0f * (1.0f - t) + 5.0f };
        switch (m_mode)
        {
        case EBlackOutLightingMode::PointDiffuse:
            m_lightEffect->SetValue(D2D1_POINTDIFFUSE_PROP_LIGHT_POSITION, lightPos);
            m_lightEffect->SetValue(D2D1_POINTDIFFUSE_PROP_DIFFUSE_CONSTANT, 1.0f - 0.9f * t);
            m_lightEffect->SetValue(D2D1_POINTDIFFUSE_PROP_SURFACE_SCALE, 0.0f);
            m_lightEffect->SetValue(D2D1_POINTDIFFUSE_PROP_COLOR, D2D1::Vector3F(1.0f, 1.0f, 1.0f));
            m_lightEffect->SetValue(D2D1_POINTDIFFUSE_PROP_KERNEL_UNIT_LENGTH, D2D1::Vector2F(1.0f, 1.0f));
            break;
        case EBlackOutLightingMode::SpotDiffuse:
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_LIGHT_POSITION, lightPos);
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_POINTS_AT, D2D1::Vector3F(center.x, center.y, 0.0f));
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_FOCUS, 0.5f + 0.4f * t);
            {
                float cone = 0.35f - 0.3f * t; if (cone < 0.03f) cone = 0.03f;
                m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_LIMITING_CONE_ANGLE, cone);
            }
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_DIFFUSE_CONSTANT, 1.0f - 0.9f * t);
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_SURFACE_SCALE, 0.0f);
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_COLOR, D2D1::Vector3F(1.0f, 1.0f, 1.0f));
            m_lightEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_KERNEL_UNIT_LENGTH, D2D1::Vector2F(1.0f, 1.0f));
            break;
        case EBlackOutLightingMode::SpotSpecular:
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIGHT_POSITION, lightPos);
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_POINTS_AT, D2D1::Vector3F(center.x, center.y, 0.0f));
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_FOCUS, 0.5f + 0.4f * t);
            {
                float cone = 0.35f - 0.3f * t; if (cone < 0.03f) cone = 0.03f;
                m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIMITING_CONE_ANGLE, cone);
            }
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_CONSTANT, 0.2f * (1.0f - t));
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_EXPONENT, 8.0f + 56.0f * t);
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SURFACE_SCALE, 0.0f);
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_COLOR, D2D1::Vector3F(1.0f, 1.0f, 1.0f));
            m_lightEffect->SetValue(D2D1_SPOTSPECULAR_PROP_KERNEL_UNIT_LENGTH, D2D1::Vector2F(1.0f, 1.0f));
            break;
        case EBlackOutLightingMode::VignetteOnly:
        default:
            break;
        }
    }

    // Opacity/CrossFade 진행
    if (m_opacityEffect)
        m_opacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, m_maxEdgeAlpha * t);
    if (m_crossFadeEffect)
        m_crossFadeEffect->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, t);
}


