#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;

// D2D 커스텀(표준) Vertex-like 효과 체인 예제 렌더러
// 실제 D2D는 VertexShader가 없으므로, TransformedImageSource/DisplacementMap 등을 조합해 기하학적 왜곡을 연출
class VertexShaderRenderer : public RenderComponent
{
public:
    VertexShaderRenderer();
    ~VertexShaderRenderer() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetSize(const FVector2& s);

private:
    void buildEffectGraph();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;
    FVector2 m_size{ 640, 360 };
    float m_time{ 0.0f };
};


