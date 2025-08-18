#include "pch.h"
#include "VertexShaderRenderer.h"

#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <d2d1effects.h>

using Microsoft::WRL::ComPtr;

VertexShaderRenderer::VertexShaderRenderer() {}
VertexShaderRenderer::~VertexShaderRenderer() {}

void VertexShaderRenderer::Initialize()
{
    __super::Initialize();
    buildEffectGraph();
}

void VertexShaderRenderer::Release()
{
    __super::Release();
    m_effect.Reset();
}

void VertexShaderRenderer::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    m_time += deltaSeconds;
    if (m_effect)
    {
        // Vignette transition size를 시간에 따라 변화시키는 등 간단한 애니메이션
        float t = 0.5f + 0.4f * sinf(m_time * 2.0f);
        m_effect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, t);
    }
}

void VertexShaderRenderer::Render()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;
    __super::Render();
    if (!m_effect) buildEffectGraph();
    if (!m_effect) return;

    const D2D1_RECT_F dst = D2D1::RectF(-m_size.x * 0.5f, -m_size.y * 0.5f, m_size.x * 0.5f, m_size.y * 0.5f);
    dc->DrawImage(m_effect.Get(), D2D1::Point2F(dst.left, dst.top));
}

float VertexShaderRenderer::GetBitmapSizeX() { return m_size.x; }
float VertexShaderRenderer::GetBitmapSizeY() { return m_size.y; }

void VertexShaderRenderer::SetSize(const FVector2& s)
{
    m_size = s;
}

void VertexShaderRenderer::buildEffectGraph()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;

    // 간단: Vignette 단일 출력 (Vertex 효과 느낌의 화면 변형 대신 트랜지션/기하감 강조)
    ComPtr<ID2D1Effect> vig;
    if (SUCCEEDED(dc->CreateEffect(CLSID_D2D1Vignette, &vig)))
    {
        vig->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, 0.6f);
        m_effect = vig;
    }
}


