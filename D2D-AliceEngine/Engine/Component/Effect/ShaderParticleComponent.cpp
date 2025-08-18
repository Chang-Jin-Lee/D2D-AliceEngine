// 표준 D2D 효과 체인을 이용하는 Shader 기반 파티클/이펙트
#include "pch.h"
#include "ShaderParticleComponent.h"

#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <d2d1effects.h>

using Microsoft::WRL::ComPtr;

ShaderParticleComponent::ShaderParticleComponent() {}
ShaderParticleComponent::~ShaderParticleComponent() {}

void ShaderParticleComponent::Initialize()
{
    __super::Initialize();
    ensureEffect();
}

void ShaderParticleComponent::Release()
{
    __super::Release();
    m_effect.Reset();
}

void ShaderParticleComponent::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    m_time += deltaSeconds;
}

void ShaderParticleComponent::Render()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;

    __super::Render();
    ensureEffect();
    if (!m_effect) return;

    dc->SetPrimitiveBlend(m_additive ? D2D1_PRIMITIVE_BLEND_ADD : D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

    // 시간 기반 파라미터 간단 갱신
    updateConstantBuffer();

    const D2D1_RECT_F dst = D2D1::RectF(-m_size.x * 0.5f, -m_size.y * 0.5f, m_size.x * 0.5f, m_size.y * 0.5f);
    dc->DrawImage(m_effect.Get(), D2D1::Point2F(dst.left, dst.top));

    dc->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}

float ShaderParticleComponent::GetBitmapSizeX() { return m_size.x; }
float ShaderParticleComponent::GetBitmapSizeY() { return m_size.y; }

void ShaderParticleComponent::SetShader(EShaderKind kind)
{
    m_kind = kind;
    m_effect.Reset();
    ensureEffect();
}

void ShaderParticleComponent::SetSize(const FVector2& size)
{
    m_size = size;
}

void ShaderParticleComponent::SetAdditive(bool additive)
{
    m_additive = additive;
}

void ShaderParticleComponent::ensureEffect()
{
    if (m_effect) return;

    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;

    // 입력용 흰색 비트맵 준비
    static ComPtr<ID2D1Bitmap1> whiteBmp;
    if (!whiteBmp)
    {
        const UINT w = 64, h = 64;
        std::vector<uint8_t> pixels(w * h * 4, 255);
        D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_NONE,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
        ComPtr<ID2D1Bitmap1> bmp;
        if (SUCCEEDED(dc->CreateBitmap(D2D1::SizeU(w, h), pixels.data(), w * 4, &props, &bmp)))
            whiteBmp = bmp;
    }

    ComPtr<ID2D1Effect> fx;
    HRESULT hr = S_OK;
    switch (m_kind)
    {
    case EShaderKind::Blending:
    {
        ComPtr<ID2D1Effect> blur, color;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1GaussianBlur, &blur)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1ColorMatrix, &color)))
        {
            blur->SetInput(0, whiteBmp.Get());
            blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 6.0f);

            const D2D1_MATRIX_5X4_F mat =
            {   1,   0,   0, 0,
                0.6f, 1,   0, 0,
                0,  0.4f, 1, 0,
                0,    0,   0, 1,
                0,    0,   0, 0 };
            color->SetInputEffect(0, blur.Get());
            color->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, mat);
            fx = color;
        }
        break;
    }
    case EShaderKind::ColorChange:
    {
        ComPtr<ID2D1Effect> hue;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1HueRotation, &hue)))
        {
            hue->SetInput(0, whiteBmp.Get());
            hue->SetValue(D2D1_HUEROTATION_PROP_ANGLE, 180.0f);
            fx = hue;
        }
        break;
    }
    case EShaderKind::Distortion:
    {
        ComPtr<ID2D1Effect> turb;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Turbulence, &turb)))
        {
            fx = turb;
        }
        break;
    }
    case EShaderKind::Special:
    {
        ComPtr<ID2D1Effect> disp, turb;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1DisplacementMap, &disp)))
        {
            disp->SetInput(0, whiteBmp.Get());
            if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Turbulence, &turb)))
            {
                disp->SetInputEffect(1, turb.Get());
            }
            fx = disp;
        }
        break;
    }
    case EShaderKind::Environment:
    {
        ComPtr<ID2D1Effect> vig;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Vignette, &vig)))
        {
            vig->SetInput(0, whiteBmp.Get());
            vig->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, 0.7f);
            fx = vig;
        }
        break;
    }
    case EShaderKind::Physics:
    {
        ComPtr<ID2D1Effect> sharp;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Sharpen, &sharp)))
        {
            sharp->SetInput(0, whiteBmp.Get());
            sharp->SetValue(D2D1_SHARPEN_PROP_SHARPNESS, 3.0f);
            fx = sharp;
        }
        break;
    }
    default: break;
    }

    if (FAILED(hr) || !fx) return;
    m_effect = fx;
}

void ShaderParticleComponent::updateConstantBuffer()
{
    if (!m_effect) return;
    m_effect->SetValue(D2D1_HUEROTATION_PROP_ANGLE, fmodf(m_time * 60.0f, 360.0f));
}

