#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>
#include <vector>
#include <random>

struct ID2D1Bitmap1;

// 간단하고 가벼운 2D 파티클 컴포넌트 (D2D SpriteBatch 사용)
// - 월드/스크린 공간 모두 지원 (RenderComponent::drawType 사용)
// - Additive/Alpha 블렌딩 스위칭
// - 여러 프리셋 이펙트 제공: 폭발, 임팩트, 클릭, 트레일, 오라 등
class ParticleComponent : public RenderComponent
{
public:
    ParticleComponent();
    ~ParticleComponent() override;

    void Initialize() override;
    void Update(const float& deltaSeconds) override;
    void Release() override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    // 커스텀 파티클 텍스처 로드 (Resource 기준 경로 또는 절대 경로)
    void LoadData(const std::wstring& relativeOrAbsolutePath);

    // 이펙트 프리셋 API -----------------------------------------------------
    void EmitExplosion(const FVector2& worldOrScreenPos, int count = 60);
    void EmitExplosionByColor(const FVector2& pos, int count, const D2D1::ColorF& colorA, const D2D1::ColorF& colorB);
    void EmitImpact(const FVector2& worldOrScreenPos, int count = 28);
    void EmitImpactByColor(const FVector2& pos, int count, D2D1::ColorF colorA, D2D1::ColorF colorB);
    void EmitClickBurst(const FVector2& pos, bool rightClick = false);
    void ToggleMouseTrail(bool enabled);
    void EmitAura(const FVector2& center, float radius = 80.0f, int count = 40);
    void EmitElectric(const FVector2& pos, int count = 45);
    void EmitPortalSwirl(const FVector2& pos, int count = 70);

    void EmitLocationIndicator(const FVector2& pos, int count = 70, D2D1::ColorF colorA = D2D1::ColorF(0.0f, 0.0f, 255.0f, 0.9f), D2D1::ColorF colorB = D2D1::ColorF(0.9f, 30.6f, 30.2f, 0.0f));

    // 프리셋 커스텀 오버로드 -----------------------------------------------
    void EmitExplosion(
        const FVector2& pos,
        int count,
        float speedMin, float speedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
        float drag, float gravity,
        bool shockwaveEnabled,
        float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
        const D2D1_COLOR_F& shockwaveColor);

    void EmitImpact(
        const FVector2& pos,
        int count,
        float speedMin, float speedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
        float drag, float gravity,
        bool shockwaveEnabled,
        float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
        const D2D1_COLOR_F& shockwaveColor);

    void EmitClickBurst(
        const FVector2& pos,
        int count,
        float speedMin, float speedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
        float drag, float gravity,
        bool enableSelfDestruct, float selfDestructAfterSeconds);

    void EmitAura(
        const FVector2& center,
        float radius,
        int count,
        float radiusJitter,
        float angularSpeedMin, float angularSpeedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        float drag, float gravity,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB);

    void EmitElectric(
        const FVector2& pos,
        int count,
        float speedMin, float speedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
        float drag, float gravity,
        float spreadRadians);

    void EmitPortalSwirl(
        const FVector2& pos,
        int count,
        float speedMin, float speedMax,
        float sizeMin, float sizeMax,
        float lifeMin, float lifeMax,
        const D2D1_COLOR_F& colorA, const D2D1_COLOR_F& colorB,
        float drag, float gravity,
        float spreadRadians);

    // 렌더링 모드
    void SetAdditiveBlend(bool enabled) { m_useAdditive = enabled; }

    // 마우스 트레일 켜고 끄기
    bool GetMouseTrail() const { return m_mouseTrail; }
    void SetMouseTrail(bool value) { m_mouseTrail = value; }
    void ToggleMouseTrail() { m_mouseTrail = !m_mouseTrail; }

private:
    // WorldSpace 입력일 때만 Y를 반전하여 내부 시뮬레이션 좌표로 변환
    FVector2 ToSimPos(const FVector2& in) const;

    struct Particle
    {
        FVector2 position;
        FVector2 velocity;
        float rotation = 0.0f;
        float angularVelocity = 0.0f;
        float size = 16.0f;       // 픽셀 기준
        float life = 0.0f;
        float maxLife = 1.0f;     // 초
        D2D1_COLOR_F colorStart{ 1,1,1,1 };
        D2D1_COLOR_F colorEnd{ 1,1,1,0 };
        float drag = 0.0f;        // 감쇠
        float gravity = 0.0f;     // +Y 방향 가정

        // 오오라: 중심을 기준으로 공전
        bool    isOrbital = false;
        FVector2 orbitalCenter{ 0,0 };
        float   orbitalRadius = 0.0f;
        float   orbitalAngularSpeed = 0.0f; // deg/sec
        float   orbitalAngleDeg = 0.0f;     // 현재 각도
    };

private:
    void emitBurstCommon(const FVector2& pos,
        int count,
        float speedMin,
        float speedMax,
        float sizeMin,
        float sizeMax,
        float lifeMin,
        float lifeMax,
        const D2D1_COLOR_F& colorA,
        const D2D1_COLOR_F& colorB,
        float drag = 0.0f,
        float gravity = 0.0f,
        float spreadRadians = 3.1415926f * 2.0f);

    void spawnShockwaveRing(const FVector2& pos, float radius, float thickness, float life, const D2D1_COLOR_F& color);

    void ensureParticleTexture();
    void createSoftCircleBitmap();

    // 유틸
    float randRange(float a, float b);
    FVector2 randDir();
    D2D1_COLOR_F lerpColor(const D2D1_COLOR_F& a, const D2D1_COLOR_F& b, float t);

private:
    std::vector<Particle> m_particles;
    std::shared_ptr<ID2D1Bitmap1> m_particleBitmap; // 부드러운 원 텍스처(프로시저 생성)
    bool m_useAdditive = true;

    // 지속 효과 토글들
    bool m_mouseTrail = false;
    float m_trailAccumulator = 0.0f; // 초

    // 현재 파티클 텍스처의 픽셀 크기 (SpriteBatch srcU 계산)
    UINT m_srcWidth = 64;
    UINT m_srcHeight = 64;

    // 클릭 버스트 자동 삭제 타이머
    bool  m_selfDestructActive = false;
    float m_selfDestructTimer = 0.0f;
    float m_selfDestructAfterSeconds = 0.0f;

    // WorldSpace 입력 좌표 Y 반전 토글
    bool m_flipWorldYInput = true;

    // 랜덤
    std::mt19937 m_rng;
};


