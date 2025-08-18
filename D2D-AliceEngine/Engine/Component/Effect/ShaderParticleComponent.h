#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;

// D2D 커스텀 PixelShader를 사용하는 파티클/이펙트 컴포넌트
// - D2DRenderManager가 런타임에 HLSL을 컴파일하고 LoadPixelShader로 로드합니다.
// - 본 컴포넌트는 DrawPixelShader 이펙트를 생성하여 사각형 영역에 그립니다.
class ShaderParticleComponent : public RenderComponent
{
public:
    enum class EShaderKind
    {
        Blending,
        ColorChange,
        Distortion,
        Special,
        Environment,
        Physics,
    };

public:
    ShaderParticleComponent();
    ~ShaderParticleComponent() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetShader(EShaderKind kind);
    void SetSize(const FVector2& size);     // 화면/월드에서 그릴 박스 크기
    void SetAdditive(bool additive);

private:
    void ensureEffect();
    void updateConstantBuffer();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;
    EShaderKind m_kind = EShaderKind::Special;
    FVector2 m_size = FVector2(256.0f, 256.0f);
    bool m_additive = true;
    float m_time = 0.0f;
};

