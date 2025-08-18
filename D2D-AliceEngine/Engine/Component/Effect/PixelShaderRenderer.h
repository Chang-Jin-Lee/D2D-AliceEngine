#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;
struct ID2D1Bitmap1;

// Direct2D 표준 효과 체인을 이용한 픽셀 이펙트 렌더러
class PixelShaderRenderer : public RenderComponent
{
public:
    enum class EMode
    {
        GlitterClick,   // 마우스 글리터 팡
        BulletImpact,   // 총알 충돌 펑
        PlayerHit,      // 플레이어 충돌
        Aura,           // 오라
        Electric,       // 전기 스파크
        Portal,         // 포탈
        BloomPulse,     // 블룸 펄스
    };

public:
    PixelShaderRenderer();
    ~PixelShaderRenderer() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetMode(EMode mode);
    void SetCenter(const FVector2& center);
    void SetSize(const FVector2& size);

private:
    void buildEffectGraph();
    void ensureWhiteBitmap();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;      // 최종 출력 이펙트
    Microsoft::WRL::ComPtr<ID2D1Effect> m_src;         // 입력 소스 (TransformedImageSource 등)
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_whiteBmp;   // 기본 입력 비트맵

    EMode m_mode{ EMode::GlitterClick };
    FVector2 m_center{ 0, 0 };
    FVector2 m_size{ 640, 360 };
    float m_time{ 0.0f };
};


