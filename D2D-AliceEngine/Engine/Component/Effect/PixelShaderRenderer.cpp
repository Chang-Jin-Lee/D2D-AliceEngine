#include "pch.h"
#include "PixelShaderRenderer.h"

#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <d2d1effects.h>

using Microsoft::WRL::ComPtr;

PixelShaderRenderer::PixelShaderRenderer() {}
PixelShaderRenderer::~PixelShaderRenderer() {}

void PixelShaderRenderer::Initialize()
{
    __super::Initialize();
    ensureWhiteBitmap();
    buildEffectGraph();
}

void PixelShaderRenderer::Release()
{
    __super::Release();
    m_effect.Reset();
    m_src.Reset();
    m_whiteBmp.Reset();
}

void PixelShaderRenderer::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    m_time += deltaSeconds;

    if (!m_effect) return;

    // 간단한 시간 기반 애니메이션 파라미터들
    switch (m_mode)
    {
    case EMode::GlitterClick:
    case EMode::BulletImpact:
    case EMode::PlayerHit:
    case EMode::Aura:
    case EMode::Electric:
    case EMode::Portal:
    case EMode::BloomPulse:
        // HueRotation이 있으면 각도 갱신 (실패 무시)
        m_effect->SetValue(D2D1_HUEROTATION_PROP_ANGLE, fmodf(m_time * 120.0f, 360.0f));
        break;
    }
}

void PixelShaderRenderer::Render()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;

    __super::Render();
    if (!m_effect) buildEffectGraph();
    if (!m_effect) return;

    // Additive Blend로 좀 더 이펙트 느낌 강화
    dc->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_ADD);
    const D2D1_RECT_F dst = D2D1::RectF(-m_size.x * 0.5f, -m_size.y * 0.5f, m_size.x * 0.5f, m_size.y * 0.5f);
    dc->DrawImage(m_effect.Get(), D2D1::Point2F(dst.left, dst.top));
    dc->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
}

float PixelShaderRenderer::GetBitmapSizeX() { return m_size.x; }
float PixelShaderRenderer::GetBitmapSizeY() { return m_size.y; }

void PixelShaderRenderer::SetMode(EMode mode)
{
    m_mode = mode;
    buildEffectGraph();
}

void PixelShaderRenderer::SetCenter(const FVector2& center)
{
    m_center = center;
}

void PixelShaderRenderer::SetSize(const FVector2& size)
{
    m_size = size;
}

void PixelShaderRenderer::ensureWhiteBitmap()
{
    if (m_whiteBmp) return;
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;
    const UINT w = 128, h = 128;
    std::vector<uint8_t> pixels(w * h * 4, 255);
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_NONE,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
    ComPtr<ID2D1Bitmap1> bmp;
    if (SUCCEEDED(dc->CreateBitmap(D2D1::SizeU(w, h), pixels.data(), w * 4, &props, &bmp)))
        m_whiteBmp = bmp;
}

void PixelShaderRenderer::buildEffectGraph()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;
    ensureWhiteBitmap();
    m_effect.Reset();

    ComPtr<ID2D1Effect> fx;
    switch (m_mode)
    {
    case EMode::GlitterClick:
    {
        // Glow + Color shift + Sharpen (팡 터지는 느낌)
        ComPtr<ID2D1Effect> blur, color, sharp;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1GaussianBlur, &blur)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1ColorMatrix, &color)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1Sharpen, &sharp)))
        {
            blur->SetInput(0, m_whiteBmp.Get());
            blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 8.0f);
            const D2D1_MATRIX_5X4_F mat = { 1,0,0,0,  0.4f,1,0,0,  0,0.2f,1,0,  0,0,0,1,  0,0,0,0 };
            color->SetInputEffect(0, blur.Get());
            color->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, mat);
            sharp->SetInputEffect(0, color.Get());
            sharp->SetValue(D2D1_SHARPEN_PROP_SHARPNESS, 2.0f);
            fx = sharp;
        }
        break;
    }
    case EMode::BulletImpact:
    {
        // DisplacementMap + Turbulence (임팩트 펑)
        ComPtr<ID2D1Effect> disp, turb;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1DisplacementMap, &disp)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1Turbulence, &turb)))
        {
            disp->SetInput(0, m_whiteBmp.Get());
            disp->SetInputEffect(1, turb.Get());
            fx = disp;
        }
        break;
    }
    case EMode::PlayerHit:
    {
        // Vignette + ColorMatrix 살짝 붉게
        ComPtr<ID2D1Effect> vig, color;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Vignette, &vig)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1ColorMatrix, &color)))
        {
            vig->SetInput(0, m_whiteBmp.Get());
            vig->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, 0.9f);
            const D2D1_MATRIX_5X4_F mat = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1,  0.2f,0,0,0 };
            color->SetInputEffect(0, vig.Get());
            color->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, mat);
            fx = color;
        }
        break;
    }
    case EMode::Aura:
    {
        // HueRotation + GaussianBlur (오라 느낌)
        ComPtr<ID2D1Effect> hue, blur;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1HueRotation, &hue)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1GaussianBlur, &blur)))
        {
            hue->SetInput(0, m_whiteBmp.Get());
            hue->SetValue(D2D1_HUEROTATION_PROP_ANGLE, 120.0f);
            blur->SetInputEffect(0, hue.Get());
            blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 5.0f);
            fx = blur;
        }
        break;
    }
    case EMode::Electric:
    {
        // Turbulence 단독 (전기 느낌 질감)
        ComPtr<ID2D1Effect> turb;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Turbulence, &turb)))
        {
            fx = turb;
        }
        break;
    }
    case EMode::Portal:
    {
        // DisplacementMap(Turbulence) + HueRotation
        ComPtr<ID2D1Effect> disp, turb, hue;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1DisplacementMap, &disp)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1Turbulence, &turb)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1HueRotation, &hue)))
        {
            disp->SetInput(0, m_whiteBmp.Get());
            disp->SetInputEffect(1, turb.Get());
            hue->SetInputEffect(0, disp.Get());
            hue->SetValue(D2D1_HUEROTATION_PROP_ANGLE, 240.0f);
            fx = hue;
        }
        break;
    }
    case EMode::BloomPulse:
    {
        // GaussianBlur 강하게 + ColorMatrix로 밝기 상승
        ComPtr<ID2D1Effect> blur, color;
        if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1GaussianBlur, &blur)) &&
            SUCCEEDED(dc->CreateEffect(CLSID_D2D1ColorMatrix, &color)))
        {
            blur->SetInput(0, m_whiteBmp.Get());
            blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 12.0f);
            const float pulse = 0.8f + 0.2f * sinf(m_time * 4.0f);
            const D2D1_MATRIX_5X4_F mat = { pulse,0,0,0,  0,pulse,0,0,  0,0,pulse,0,  0,0,0,1,  0,0,0,0 };
            color->SetInputEffect(0, blur.Get());
            color->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, mat);
            fx = color;
        }
        break;
    }
    }

    m_effect = fx;
}


